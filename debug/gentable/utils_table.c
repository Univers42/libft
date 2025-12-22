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
    
    db = xcalloc(1, sizeof(t_database));
    if (!db)
		return (NULL);
    ft_setlocale(FT_LC_CTYPE, "");
    db->title = title ? ft_strdup(title) : ft_strdup("Untitled Database");
    db->cols_capacity = INITIAL_CAPACITY;
    db->rows_capacity = INITIAL_CAPACITY;
    db->cols = xcalloc(db->cols_capacity, sizeof(t_col));
    db->rows = xcalloc(db->rows_capacity, sizeof(t_row));
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
        size_t newcap = db->cols_capacity * GROWTH_FACTOR;
        new_cols = realloc(db->cols, newcap * sizeof(t_col));
        if (!new_cols) return -1;
        db->cols = new_cols;
        db->cols_capacity = newcap;
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

int db_add_row_with_label(t_database *db, const char *label, const char **values, size_t nvalues)
{
    t_row *new_rows;
    size_t i;
    
    if (!db) return -1;
    
    if (db->nrows >= db->rows_capacity) {
        size_t newcap = db->rows_capacity * GROWTH_FACTOR;
        new_rows = realloc(db->rows, newcap * sizeof(t_row));
        if (!new_rows) return -1;
        db->rows = new_rows;
        db->rows_capacity = newcap;
    }
    
    db->rows[db->nrows].label = label ? ft_strdup(label) : NULL;
    db->rows[db->nrows].data = xcalloc(db->ncols, sizeof(char *));
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

int db_add_row(t_database *db, ...)
{
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

int db_load_from_csv(t_database *db, const char *filename)
{
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

    /* Accumulate possibly-multiline CSV records (quoted fields may contain newlines) */
    char *acc = NULL;
    size_t acc_len = 0;

    while (fgets(line, sizeof(line), fp)) {
        size_t line_len = ft_strlen(line);
        /* strip trailing newline */
        if (line_len && line[line_len - 1] == '\n') {
            line[--line_len] = '\0';
            line_len--;
            /* above reduces by one extra to keep logic simple; ensure non-negative handled */
            if ((int)line_len < 0) line_len = 0;
        }

        /* append line to accumulator */
        char *new_acc = realloc(acc, acc_len + line_len + 2); /* +1 for possible separator, +1 for '\0' */
        if (!new_acc) {
            free(acc);
            fclose(fp);
            return -1;
        }
        acc = new_acc;
        /* if acc was empty, start at 0 */
        if (acc_len == 0)
            acc[0] = '\0';
        /* if accumulator not empty, append newline that was removed (preserve original newlines inside quotes) */
        if (acc_len > 0) {
            acc[acc_len++] = '\n';
            acc[acc_len] = '\0';
        }
        /* append current line */
        memcpy(acc + acc_len, line, line_len);
        acc_len += line_len;
        acc[acc_len] = '\0';

        /* check CSV quote balance on acc: parse, handling doubled quotes ("") */
        bool in_quotes = false;
        const char *p = acc;
        while (*p) {
            if (*p == '"') {
                if (p[1] == '"') { p += 2; continue; } /* escaped quote */
                in_quotes = !in_quotes;
                p++;
                continue;
            }
            p++;
        }
        if (in_quotes) {
            /* record not finished; read next physical line */
            continue;
        }

        /* acc contains a complete CSV record; split it */
        if (ft_strlen(acc) == 0) {
            free(acc);
            acc = NULL;
            acc_len = 0;
            continue;
        }

        fields = csv_split(acc, db->config.col_separator);
        if (!fields) {
            free(acc);
            acc = NULL;
            acc_len = 0;
            continue;
        }

        /* Count fields */
        field_count = 0;
        while (fields[field_count]) field_count++;

        if (first_line) {
            /* First line = headers */
            for (i = 0; i < field_count; i++) {
                char *trimmed = trim_whitespace(fields[i]);
                db_add_column(db, trimmed, TYPE_STRING, ALIGN_LEFT);
            }
            first_line = false;
        } else {
            /* Data rows */
            const char **data = (const char **)fields;
            db_add_row_with_label(db, NULL, data, field_count);
        }

        free_split(fields);
        free(acc);
        acc = NULL;
        acc_len = 0;
    }

    /* cleanup any dangling accumulator */
    free(acc);

    fclose(fp);
    printf("Loaded %zu rows from '%s'\n", db->nrows, filename);
    return 0;
}
