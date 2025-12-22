#include "database.h"

int	db_add_row_with_label(t_database *db, const char *label,
		const char **values, size_t nvalues)
{
	t_row	*new_rows;
	size_t	i;
	size_t	newcap;
	
	if (!db)
		return (-1);
	if (db->nrows >= db->rows_capacity)
	{
		newcap = db->rows_capacity * GROWTH_FACTOR;
		new_rows = realloc(db->rows, newcap * sizeof(t_row));
		if (!new_rows)
			return (-1);
		db->rows = new_rows;
		db->rows_capacity = newcap;
	}
	db->rows[db->nrows].label = NULL;
	if (label)
		db->rows[db->nrows].label = ft_strdup(label);
	db->rows[db->nrows].data = xcalloc(db->ncols, sizeof(char *));
	if (!db->rows[db->nrows].data)
		return (-1);
	db->rows[db->nrows].ncols = db->ncols;
	i = -1;
	while (++i < db->ncols)
	{
		if (i < nvalues && values[i])
			db->rows[db->nrows].data[i] = ft_strdup(values[i]);
		else
			db->rows[db->nrows].data[i] = ft_strdup("");
	}
	db->nrows++;
	return (0);
}

