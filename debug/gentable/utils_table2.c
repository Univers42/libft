#include "database.h"

void	calculate_column_widths(t_database *db, size_t *widths, size_t *label_width)
{
	size_t	i;
	size_t	j;
	int		len;

	*label_width = 0;
	if (db->config.show_left_attributes)
	{
		for (i = 0; i < db->nrows; i++)
		{
			if (db->rows[i].label)
			{
				len = display_width(db->rows[i].label);
				*label_width = max_size(*label_width, (size_t)len);
			}
		}
		*label_width = max_size(*label_width, 8);
	}
	i = -1;
	while (++i < db->ncols)
		widths[i] = display_width(db->cols[i].name);
	i = -1;
	while (++i < db->nrows)
	{
		j = -1;
		while (++j < db->ncols && j < db->rows[i].ncols)
		{
			if (db->rows[i].data[j])
			{
				len = display_width(db->rows[i].data[j]);
				widths[j] = max_size(widths[j], (size_t)len);
			}
		}
	}
	i = -1;
	while (++i < db->ncols)
	{
		if (db->cols[i].width > 0)
			widths[i] = db->cols[i].width;
	}
}

// return displayed column width (wcwidth-aware) of a UTF-8 string
int	display_width(const char *s)
{
	mbstate_t			st;
	const unsigned char	*p = (const unsigned char *)s;
	size_t				bytes;
	wchar_t				wc;
	int					w;
	int					cw;

	w = 0;
	ft_memset(&st, 0, sizeof(st));
	while (*p)
	{
		bytes = ft_mbrtowc(&wc, (const char *)p, ft_mb_cur_max(), &st);
		if (bytes == (size_t)-1 || bytes == (size_t)-2 || bytes == 0)
		{
			w += 1;
			p++;
			ft_memset(&st, 0, sizeof(st));
			continue ;
		}
		cw = ft_wcwidth(wc);
		if (cw < 0)
			cw = 0;
		w += cw;
		p += bytes;
	}
	return (w);
}
