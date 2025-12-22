#include "database.h"

t_database	*db_create(const char *title)
{
	t_database	*db;
	
	db = xcalloc(1, sizeof(t_database));
	if (!db)
		return (NULL);
	ft_setlocale(FT_LC_CTYPE, "");
	if (title)
		db->title = ft_strdup(title);
	db->title = ft_strdup("Untitled Database");
	db->cols_capacity = INITIAL_CAPACITY;
	db->rows_capacity = INITIAL_CAPACITY;
	db->cols = xcalloc(db->cols_capacity, sizeof(t_col));
	db->rows = xcalloc(db->rows_capacity, sizeof(t_row));
	if (!db->cols || !db->rows)
		return (free(db->title), free(db->cols),
			free(db->rows), free(db), NULL);
	db->style = default_style();
	db->config = default_config();
	return (db);
}
