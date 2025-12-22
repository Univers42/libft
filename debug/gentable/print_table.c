#include "database.h"

/* copy from src to dst up to max_display_width display columns, preserving UTF-8 char boundaries */
void utf8_truncate_by_display_width(const char *src, size_t max_display, char *dst, size_t dst_size)
{
	mbstate_t st;
	ft_memset(&st, 0, sizeof(st));
	const unsigned char *p = (const unsigned char *)src;
	size_t bytes;
	wchar_t wc;
	int acc = 0;
	size_t out = 0;

	if (!src || !dst || dst_size == 0) return;
	dst[0] = '\0';

	while (*p)
	{
		bytes = ft_mbrtowc(&wc, (const char *)p, ft_mb_cur_max(), &st);
		if (bytes == (size_t)-1 || bytes == (size_t)-2 || bytes == 0)
		{
			// invalid byte -> treat single byte as width 1
			if (acc + 1 > (int)max_display) break;
			if (out + 1 < dst_size) dst[out++] = *p;
			acc += 1;
			p++;
			ft_memset(&st, 0, sizeof(st));
			continue;
		}
		int cw = ft_wcwidth(wc);
		if (cw < 0) cw = 0;
		if (acc + cw > (int)max_display) break;
		if (out + bytes < dst_size)
		{
			ft_memcpy(dst + out, p, bytes);
			out += bytes;
		}
		acc += cw;
		p += bytes;
	}
	if (out < dst_size) dst[out] = '\0';
	else dst[dst_size - 1] = '\0';
}

// ============= TABLE RENDERING =============

void print_separator(size_t *widths, size_t ncols, size_t label_width,
						   bool show_label, const char *left, const char *mid, 
						   const char *right, const char *line)
{
	size_t i, j;
	
	ft_printf("%s", left);
	
	if (show_label) {
		for (j = 0; j < label_width + 2; j++) {
			ft_printf("%s", line);
		}
		ft_printf("%s", mid);
	}
	
	for (i = 0; i < ncols; i++) {
		for (j = 0; j < widths[i] + 2; j++) {
			ft_printf("%s", line);
		}
		if (i < ncols - 1) {
			ft_printf("%s", mid);
		}
	}
	ft_printf("%s\n", right);
}

void db_render(t_database *db)
{
	size_t *widths;
	size_t label_width = 0;
	size_t i, j;
	char cell_buf[MAX_LINE_LENGTH];
	t_align align;

	if (!db || db->ncols == 0)
		return;
	widths = xcalloc(db->ncols, sizeof(size_t));
	if (!widths)
		return;
	calculate_column_widths(db, widths, &label_width);
	if (db->config.show_title && db->title)
	{
		ft_printf("\n");
		if (db->config.use_bold_header)
			ft_printf(ASCII_BOLD);
		print_rgb_color(db->style.title);
		ft_printf("%s", db->title);
		ft_printf(ASCII_RESET);
		ft_printf("\n");
	}
	print_separator(widths, db->ncols, label_width, db->config.show_left_attributes,
				   SYMBOL_UNICODE_HEADER_TOP_LEFT, SYMBOL_UNICODE_HEADER_T_JOINT,
				   SYMBOL_UNICODE_HEADER_TOP_RIGHT, SYMBOL_UNICODE_HEADER_HORIZONTAL_LINE);
	if (db->config.show_header)
	{
		ft_printf("%s", SYMBOL_UNICODE_BAR);
		if (db->config.show_left_attributes) {
			{
				t_pad_ctx ctx = { .buf = cell_buf, .text = "Attribute",
					.width = label_width, .align = ALIGN_CENTER,
					.bufsize = sizeof(cell_buf), .dlen = 0 };
				format_cell(&ctx);
			}
			if (db->config.use_bold_header)
				ft_printf(ASCII_BOLD);
			print_rgb_color(db->style.header);
			ft_printf(" %s ", cell_buf);
			ft_printf(ASCII_RESET);
			ft_printf("%s", SYMBOL_UNICODE_BAR);
		}
		
		for (i = 0; i < db->ncols; i++) {
			{
				t_pad_ctx ctx = { .buf = cell_buf, .text = db->cols[i].name,
					.width = widths[i], .align = ALIGN_CENTER,
					.bufsize = sizeof(cell_buf), .dlen = 0 };
				format_cell(&ctx);
			}
			if (db->config.use_bold_header)
				ft_printf(ASCII_BOLD);
			print_rgb_color(db->style.header);
			ft_printf(" %s ", cell_buf);
			ft_printf(ASCII_RESET);
			ft_printf("%s", SYMBOL_UNICODE_BAR);
		}
		ft_printf("\n");
		print_separator(widths, db->ncols, label_width, db->config.show_left_attributes,
					   SYMBOL_UNICODE_VERTICAL_T_LEFT, SYMBOL_UNICODE_HEADER_CROSS,
					   SYMBOL_UNICODE_VERTICAL_T_RIGHT, SYMBOL_UNICODE_HEADER_HORIZONTAL_LINE);
	}
	for (i = 0; i < db->nrows; i++)
	{
		ft_printf("%s", SYMBOL_UNICODE_BAR);
		if (db->config.show_left_attributes)
		{
			const char *label = db->rows[i].label ? db->rows[i].label : "";
			{
				t_pad_ctx ctx = { .buf = cell_buf, .text = label,
					.width = label_width, .align = ALIGN_LEFT,
					.bufsize = sizeof(cell_buf), .dlen = 0 };
				format_cell(&ctx);
			}
			if (db->config.alternating_colors)
				print_rgb_color(i % 2 == 0 ? db->style.even_row : db->style.odd_row);
			ft_printf(" %s ", cell_buf);
			if (db->config.alternating_colors)
				ft_printf(ASCII_RESET);
			ft_printf("%s", SYMBOL_UNICODE_BAR);
		}
		for (j = 0; j < db->ncols; j++)
		{
			if (db->cols[j].align != ALIGN_LEFT)
				align = db->cols[j].align;
			else if (db->cols[j].format == TYPE_INT || db->cols[j].format == TYPE_FLOAT)
				align = db->style.number_align;
			else
				align = db->style.string_align;
			const char *cell_data = "";
			if (j < db->rows[i].ncols && db->rows[i].data[j])
				cell_data = db->rows[i].data[j];
			{
				t_pad_ctx ctx = { .buf = cell_buf, .text = cell_data,
					.width = widths[j], .align = align,
					.bufsize = sizeof(cell_buf), .dlen = 0 };
				format_cell(&ctx);
			}
			if (db->config.alternating_colors)
				print_rgb_color(i % 2 == 0 ? db->style.even_row : db->style.odd_row);
			ft_printf(" %s ", cell_buf);
			if (db->config.alternating_colors)
				ft_printf(ASCII_RESET);
			ft_printf("%s", SYMBOL_UNICODE_BAR);
		}
		ft_printf("\n");
	}
	print_separator(widths, db->ncols, label_width, db->config.show_left_attributes,
				   SYMBOL_UNICODE_HEADER_BOTTOM_LEFT, SYMBOL_UNICODE_FOOTER_T_JOINT,
				   SYMBOL_UNICODE_HEADER_BOTTOM_RIGHT, SYMBOL_UNICODE_HEADER_HORIZONTAL_LINE);
	if (db->config.show_footer && db->footer)
	{
		print_rgb_color(db->style.footer);
		ft_printf("%s", db->footer);
		ft_printf(ASCII_RESET);
		ft_printf("\n");
	}
	
	free(widths);
}
