#include "database.h"

int	db_add_column(t_database *db, const char *name,
		t_type format, t_align align)
{
	t_col	*new_cols;
	size_t	i;
	size_t	new_cap;
	char	**new_data;

	if (!db || !name)
		return (-1);
	if (db->ncols >= db->cols_capacity)
	{
		new_cap = db->cols_capacity * GROWTH_FACTOR;
		new_cols = realloc(db->cols, new_cap * sizeof(t_col));
		if (!new_cols) return -1;
		db->cols = new_cols;
		db->cols_capacity = new_cap;
	}
	db->cols[db->ncols].name = ft_strdup(name);
	db->cols[db->ncols].format = format;
	db->cols[db->ncols].align = align;
	db->cols[db->ncols].width = 0;
	i = -1;
	while (++i < db->nrows)
	{
		new_data = realloc(db->rows[i].data,
				(db->ncols + 1) * sizeof(char *));
		if (!new_data)
			return (-1);
		db->rows[i].data = new_data;
		db->rows[i].data[db->ncols] = ft_strdup("");
		db->rows[i].ncols = db->ncols + 1;
	}
	db->ncols++;
	return (0);
}

