#include "database.h"

int	db_add_row(t_database *db, ...)
{
	va_list		args;
	t_row		*new_rows;
	size_t		i;
	const char	*value;
	
	if (!db)
		return (-1);
	if (db->nrows >= db->rows_capacity)
	{
		db->rows_capacity *= GROWTH_FACTOR;
		new_rows = realloc(db->rows, db->rows_capacity * sizeof(t_row));
		if (!new_rows) return -1;
		db->rows = new_rows;
	}
	db->rows[db->nrows].label = NULL;
	db->rows[db->nrows].data = calloc(db->ncols, sizeof(char *));
	if (!db->rows[db->nrows].data)
		return (-1);
	db->rows[db->nrows].ncols = db->ncols;
	// initialize id to -1 (unset)
	db->rows[db->nrows].id = -1;
	va_start(args, db);
	for (i = 0; i < db->ncols; ++i)
	{
		value = va_arg(args, const char *);
		db->rows[db->nrows].data[i] = value ? ft_strdup(value) : ft_strdup("");
	}
	va_end(args);
	// try to set id from first column if available
	if (db->config.auto_increment_id && db->ncols > 0)
	{
		int newid = db_next_auto_id(db);
		char *s = ft_itoa(newid);
		if (s)
		{
			free(db->rows[db->nrows].data[0]);
			db->rows[db->nrows].data[0] = s;
			db->rows[db->nrows].id = newid;
		}
	}
	else if (db->ncols > 0 && db->rows[db->nrows].data[0] && db->rows[db->nrows].data[0][0] != '\0')
		db->rows[db->nrows].id = ft_atoi(db->rows[db->nrows].data[0]);
	db->nrows++;
	return (0);
}
