/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_table.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 18:23:39 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/22 19:18:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "database.h"
#include <stdlib.h>

// find index of a column pointer within the database cols array
static ssize_t find_col_index(t_database *db, t_col *col)
{
    size_t i;

    if (!db || !col)
        return -1;
    for (i = 0; i < db->ncols; ++i)
    {
        if (&db->cols[i] == col)
            return (ssize_t)i;
    }
    return -1;
}

// set a cell value (row, col) to a new string (makes a copy)
int db_set_cell(t_database *db, size_t row, size_t col, const char *value)
{
    char *newv;

    if (!db)
        return -1;
    if (row >= db->nrows || col >= db->ncols)
        return -1;
    newv = value ? ft_strdup(value) : ft_strdup("");
    if (!newv)
        return -1;
    free(db->rows[row].data[col]);
    db->rows[row].data[col] = newv;
    return 0;
}

// map a transformation function over every cell of a column using ft_strmapi
int db_map_column(t_database *db, size_t col_idx, char (*f)(unsigned int, char))
{
    size_t i;
    char *old;
    char *mapped;

    if (!db || !f)
        return -1;
    if (col_idx >= db->ncols)
        return -1;
    for (i = 0; i < db->nrows; ++i)
    {
        if (!db->rows[i].data)
            continue;
        if (col_idx >= db->rows[i].ncols)
            continue;
        old = db->rows[i].data[col_idx];
        if (!old)
            old = "";
        mapped = ft_strmapi(old, f);
        if (!mapped)
            return -1;
        free(db->rows[i].data[col_idx]);
        db->rows[i].data[col_idx] = mapped;
    }
    return 0;
}

// add an integer value to a given column at a specific row index
// column is passed as a pointer to the t_col within db->cols
int add_value_idx_column(t_database *db, t_col *col, int idx, int val)
{
    ssize_t col_idx;
    char *sval;

    if (!db || !col)
        return -1;
    col_idx = find_col_index(db, col);
    if (col_idx < 0)
        return -1;
    if (idx < 0 || (size_t)idx >= db->nrows)
        return -1;
    sval = ft_itoa(val);
    if (!sval)
        return -1;
    free(db->rows[idx].data[col_idx]);
    db->rows[idx].data[col_idx] = sval;
    // also update numeric id if this is the first column
    if (col_idx == 0)
        db->rows[idx].id = val;
    return 0;
}

// add or replace a string value at given line number and column
int add_value(t_database *db, int lineno, t_col *col, char *value)
{
    ssize_t col_idx;
    size_t row_idx;
    char *newv;

    if (!db || !col || !value)
        return -1;
    if (lineno < 0)
        return -1;
    row_idx = (size_t)lineno;
    if (row_idx >= db->nrows)
        return -1;
    col_idx = find_col_index(db, col);
    if (col_idx < 0)
        return -1;
    newv = ft_strdup(value);
    if (!newv)
        return -1;
    free(db->rows[row_idx].data[col_idx]);
    db->rows[row_idx].data[col_idx] = newv;
    if (col_idx == 0)
        db->rows[row_idx].id = ft_atoi(newv);
    return 0;
}

// check whether a given id value already exists in a specified column (compare strings or numeric id)
bool is_id_already_existing(t_database *db, const char *id_value, size_t id_col)
{
    size_t i;

    if (!db || !id_value)
        return false;
    if (id_col >= db->ncols)
        return false;
    // try parse numeric id_value
    char *endptr = NULL;
    long v = strtol(id_value, &endptr, 10);
    bool is_number = (endptr != id_value && *endptr == '\0');

    for (i = 0; i < db->nrows; ++i)
    {
        if (is_number)
        {
            if (db->rows[i].id == (int)v)
                return true;
            continue;
        }
        if (db->rows[i].data && id_col < db->rows[i].ncols && db->rows[i].data[id_col])
        {
            if (ft_strcmp(db->rows[i].data[id_col], id_value) == 0)
                return true;
        }
    }
    return false;
}

// produce next auto id (simple counter based on existing max id + 1)
int db_next_auto_id(t_database *db)
{
    size_t i;
    int max = 0;

    if (!db)
        return 1;
    for (i = 0; i < db->nrows; ++i)
    {
        if (db->rows[i].id > max)
            max = db->rows[i].id;
    }
    return max + 1;
}

