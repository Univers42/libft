#include "database.h"

t_format_style	default_style(void)
{
	t_format_style	style;

	style.number_align = ALIGN_RIGHT;
	style.string_align = ALIGN_LEFT;
	style.title = make_color(220, 220, 100);
	style.header = make_color(144, 144, 144);
	style.footer = make_color(10, 255, 150);
	style.even_row = make_color(250, 250, 255);
	style.odd_row = make_color(255, 255, 255);
	return (style);
}

t_display_config	default_config(void)
{
	t_display_config	cfg;

	cfg.show_title = true;
	cfg.show_header = true;
	cfg.show_footer = true;
	cfg.show_row_numbers = false;
	cfg.show_left_attributes = false;
	cfg.alternating_colors = true;
	cfg.use_bold_header = true;
	cfg.col_separator = ',';
	return (cfg);
}
