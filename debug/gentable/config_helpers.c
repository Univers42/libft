#include "database.h"

// ============= CONFIGURATION HELPERS =============

void	db_set_style(t_database *db, t_format_style style)
{
	if (db)
		db->style = style;
}

void	db_set_config(t_database *db, t_display_config config)
{
	if (db)
		db->config = config;
}

void	db_set_footer(t_database *db, const char *footer)
{
	if (!db)
		return ;
	free(db->footer);
	db->footer = ft_strdup(footer);
}

void	db_free(t_database *db)
{
	size_t	i;
	size_t	j;

	if (!db)
		return ;
	free(db->title);
	free(db->footer);
	i = -1;
	while (++i < db->ncols)
		free(db->cols[i].name);
	free(db->cols);
	i = -1;
	while (++i < db->nrows)
	{
		free(db->rows[i].label);
		j = -1;
		while (++j < db->rows[i].ncols)
			free(db->rows[i].data[j]);
		free(db->rows[i].data);
	}
	free(db->rows);
	free(db);
}
