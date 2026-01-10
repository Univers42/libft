#include "database.h"
#include "libft.h"

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
	db->rows[db->nrows].id = -1;
	i = -1;
	while (++i < db->ncols)
	{
		if (i < nvalues && values[i])
			db->rows[db->nrows].data[i] = ft_strdup(values[i]);
		else
			db->rows[db->nrows].data[i] = ft_strdup("");
	}
	// try to set id from first column if available
	if (db->config.auto_increment_id && db->ncols > 0)
	{
		int newid = db_next_auto_id(db);
		// write newid into first column string
		char *s = ft_itoa(newid);
		if (s)
		{
			free(db->rows[db->nrows].data[0]);
			db->rows[db->nrows].data[0] = s;
			db->rows[db->nrows].id = newid;
		}
	}
	else if (db->ncols > 0 && db->rows[db->nrows].data[0])
	{
		char *endptr = NULL;
		long v = strtol(db->rows[db->nrows].data[0], &endptr, 10);
		if (endptr != db->rows[db->nrows].data[0])
			db->rows[db->nrows].id = (int)v;
	}
	db->nrows++;
	return (0);
}

