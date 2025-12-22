/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:13:14 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:13:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "database.h"

// ============= CONFIGURATION HELPERS =============

void db_set_style(t_database *db, t_format_style style)
{
    if (db) db->style = style;
}

void db_set_config(t_database *db, t_display_config config)
{
    if (db) db->config = config;
}

void db_set_footer(t_database *db, const char *footer)
{
    if (!db) return;
    free(db->footer);
    db->footer = footer ? ft_strdup(footer) : NULL;
}


// ============= CLEANUP =============

void db_free(t_database *db)
{
    size_t i, j;
    
    if (!db)
		return;
    free(db->title);
    free(db->footer);
    for (i = 0; i < db->ncols; i++)
        free(db->cols[i].name);
    free(db->cols);
    for (i = 0; i < db->nrows; i++)
	{
        free(db->rows[i].label);
        for (j = 0; j < db->rows[i].ncols; j++)
            free(db->rows[i].data[j]);
        free(db->rows[i].data);
    }
    free(db->rows);
    free(db);
}