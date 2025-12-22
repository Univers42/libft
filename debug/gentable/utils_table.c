/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_table.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:09:55 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:09:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "database.h"

t_database	*db_create(const char *title)
{
    t_database	*db;
    
    db = calloc(1, sizeof(t_database));
    if (!db) return NULL;
    setlocale(LC_CTYPE, "");
    db->title = title ? ft_strdup(title) : ft_strdup("Untitled Database");
    db->cols_capacity = INITIAL_CAPACITY;
    db->rows_capacity = INITIAL_CAPACITY;
    db->cols = calloc(db->cols_capacity, sizeof(t_col));
    db->rows = calloc(db->rows_capacity, sizeof(t_row));
    if (!db->cols || !db->rows)
	{
        free(db->title);
        free(db->cols);
        free(db->rows);
        free(db);
        return (NULL);
    }
    db->style = default_style();
    db->config = default_config();
    return (db);
}

int	db_add_column(t_database *db, const char *name, t_type format, t_align align)
{
    t_col	*new_cols;
    size_t	i;
    
    if (!db || !name)
		return (-1);
    if (db->ncols >= db->cols_capacity)
	{
        db->cols_capacity *= GROWTH_FACTOR;
        new_cols = realloc(db->cols, db->cols_capacity * sizeof(t_col));
        if (!new_cols) return -1;
        db->cols = new_cols;
    }
    db->cols[db->ncols].name = ft_strdup(name);
    db->cols[db->ncols].format = format;
    db->cols[db->ncols].align = align;
    db->cols[db->ncols].width = 0;
    for (i = 0; i < db->nrows; i++)
	{
        char **new_data = realloc(db->rows[i].data, (db->ncols + 1) * sizeof(char *));
        if (!new_data) return -1;
        db->rows[i].data = new_data;
        db->rows[i].data[db->ncols] = ft_strdup("");
        db->rows[i].ncols = db->ncols + 1;
    }
    db->ncols++;
    return (0);
}

int db_add_row_with_label(t_database *db, const char *label, const char **values, size_t nvalues) {
    t_row *new_rows;
    size_t i;
    
    if (!db) return -1;
    
    if (db->nrows >= db->rows_capacity) {
        db->rows_capacity *= GROWTH_FACTOR;
        new_rows = realloc(db->rows, db->rows_capacity * sizeof(t_row));
        if (!new_rows) return -1;
        db->rows = new_rows;
    }
    
    db->rows[db->nrows].label = label ? ft_strdup(label) : NULL;
    db->rows[db->nrows].data = calloc(db->ncols, sizeof(char *));
    if (!db->rows[db->nrows].data) return -1;
    db->rows[db->nrows].ncols = db->ncols;
    
    for (i = 0; i < db->ncols; i++) {
        if (i < nvalues && values[i]) {
            db->rows[db->nrows].data[i] = ft_strdup(values[i]);
        } else {
            db->rows[db->nrows].data[i] = ft_strdup("");
        }
    }
    
    db->nrows++;
    return 0;
}

int db_add_row(t_database *db, ...) {
    va_list args;
    t_row *new_rows;
    size_t i;
    
    if (!db) return -1;
    
    if (db->nrows >= db->rows_capacity) {
        db->rows_capacity *= GROWTH_FACTOR;
        new_rows = realloc(db->rows, db->rows_capacity * sizeof(t_row));
        if (!new_rows) return -1;
        db->rows = new_rows;
    }
    
    db->rows[db->nrows].label = NULL;
    db->rows[db->nrows].data = calloc(db->ncols, sizeof(char *));
    if (!db->rows[db->nrows].data) return -1;
    db->rows[db->nrows].ncols = db->ncols;
    
    va_start(args, db);
    for (i = 0; i < db->ncols; i++) {
        const char *val = va_arg(args, const char *);
        db->rows[db->nrows].data[i] = val ? ft_strdup(val) : ft_strdup("");
    }
    va_end(args);
    
    db->nrows++;
    return 0;
}

int db_load_from_csv(t_database *db, const char *filename) {
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    char **fields;
    size_t i, field_count;
    bool first_line = true;
    
    if (!db || !filename) return -1;
    
    fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return -1;
    }
    
    while (fgets(line, sizeof(line), fp)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        
        fields = ft_split(line, db->config.col_separator);
        if (!fields) continue;
        
        // Count fields
        field_count = 0;
        while (fields[field_count]) field_count++;
        
        if (first_line) {
            // First line = headers
            for (i = 0; i < field_count; i++) {
                char *trimmed = trim_whitespace(fields[i]);
                db_add_column(db, trimmed, TYPE_STR, ALIGN_LEFT);
            }
            first_line = false;
        } else {
            // Data rows
            const char **data = (const char **)fields;
            db_add_row_with_label(db, NULL, data, field_count);
        }
        
        free_split(fields);
    }
    
    fclose(fp);
    printf("Loaded %zu rows from '%s'\n", db->nrows, filename);
    return 0;
}
