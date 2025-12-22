#include "database.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ft_debug.h"
#include "ft_stddef.h"

static int	open_read_fd(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	return (fd);
}

static size_t	trim_line_newline(char *line)
{
	size_t	len;
	size_t	nl;

	if (!line)
		return (0);
	len = ft_strlen(line);
	nl = ft_strcspn(line, "\n");
	if (nl < len)
	{
		line[nl] = '\0';
		len = nl;
	}
	return (len);
}

static char	*append_to_acc(char *acc, const char *line, size_t line_len)
{
	size_t	acc_len;
	char	*new_acc;

	acc_len = ft_strlen(acc);
	new_acc = realloc(acc, acc_len + line_len + 2);
	if (!new_acc)
		return (free(acc), NULL);
	acc = new_acc;
	if (acc_len == 0)
		acc[0] = '\0';
	if (acc_len > 0)
	{
		acc[acc_len++] = '\n';
		acc[acc_len] = '\0';
	}
	if (line_len > 0)
	{
		ft_memcpy(acc + acc_len, line, line_len);
		acc_len += line_len;
		acc[acc_len] = '\0';
	}
	return (acc);
}

static int	acc_is_balanced(const char *acc)
{
	const char	*p;
	int			in_quotes;

	if (!acc)
		return (1);
	p = acc;
	in_quotes = 0;
	while (*p)
	{
		if (*p == '"')
		{
			if (p[1] == '"')
				p += 2;
			else
			{
				in_quotes = !in_quotes;
				p++;
			}
		}
		else
			p++;
	}
	if (in_quotes)
		return (0);
	return (1);
}

static int	process_record(t_database *db, char *acc, int *first_line)
{
	char	**fields;
	size_t	field_count;
	size_t	i;
	char	*trimmed;

	fields = csv_split(acc, db->config.col_separator);
	ft_assert(fields != NULL);
	field_count = 0;
	while (fields[field_count])
		field_count++;
	if (*first_line)
	{
		// if auto-increment requested, check whether the CSV headers already contain an 'id' column
		bool has_id = false;
		size_t k = 0;
		while (fields[k])
		{
			char *t = trim_whitespace(fields[k]);
			if (t && strcasecmp(t, "id") == 0)
			{
				has_id = true;
				break;
			}
			k++;
		}
		if (db->config.auto_increment_id && !has_id)
		{
			// create an id column at the front
			db_add_column(db, "id", TYPE_INT, ALIGN_RIGHT);
		}
		i = -1;
		while (++i < field_count)
		{
			trimmed = trim_whitespace(fields[i]);
			db_add_column(db, trimmed, TYPE_STRING, ALIGN_LEFT);
		}
		*first_line = 0;
	}
	else
	{
		/* if we inserted an 'id' column at the front earlier, the db->ncols will be
		   one greater than the number of fields. In that case we need to prepend an
		   empty value so that the CSV fields align with the columns (the id will be
		   generated later when auto_increment_id is enabled). */
		if (db->config.auto_increment_id && db->ncols == field_count + 1 && db->cols[0].name && strcasecmp(db->cols[0].name, "id") == 0)
		{
			char **new_fields;
			size_t j;

			new_fields = malloc((field_count + 2) * sizeof(char *));
			if (!new_fields)
			{
				free_split(fields);
				return (-1);
			}
			/* first element is empty string placeholder */
			new_fields[0] = ft_strdup("");
			for (j = 0; j < field_count; ++j)
				new_fields[j + 1] = fields[j];
			new_fields[field_count + 1] = NULL;

			db_add_row_with_label(db, NULL, (const char **)new_fields, field_count + 1);

			/* free temporary placeholder; original field strings are freed by free_split below */
			free(new_fields[0]);
			free(new_fields);
			free_split(fields);
			return (0);
		}
		else
		{
			db_add_row_with_label(db, NULL, (const char **)fields, field_count);
		}
	}
	free_split(fields);
	return (0);
}

static int	handle_acc(t_database *db, char **pacc, int *first_line)
{
	if (!*pacc || ft_strlen(*pacc) == 0)
	{
		free(*pacc);
		*pacc = NULL;
		return (0);
	}
	process_record(db, *pacc, first_line);
	free(*pacc);
	*pacc = NULL;
	return (0);
}

static int	process_file(int fd, t_database *db)
{
	char	*acc;
	char	*line;
	int		first_line;
	size_t	len;

	acc = NULL;
	first_line = 1;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		len = trim_line_newline(line);
		acc = append_to_acc(acc, line, len);
		free(line);
		if (!acc)
			return (-1);
		if (!acc_is_balanced(acc))
			continue ;
		handle_acc(db, &acc, &first_line);
	}
	free(acc);
	return (0);
}

static void infer_column_types(t_database *db)
{
    size_t j, i;
    if (!db)
        return;
    for (j = 0; j < db->ncols; ++j)
    {
        bool any_float = false;
        bool any_non_numeric = false;
        for (i = 0; i < db->nrows; ++i)
        {
            if (!db->rows[i].data) continue;
            if (j >= db->rows[i].ncols) continue;
            char *s = db->rows[i].data[j];
            if (!s || s[0] == '\0') continue;
            char *p = s;
            bool has_digit = false;
            bool has_dot = false;
            bool has_alpha = false;
            // allow leading +/-
            if (*p == '+' || *p == '-') p++;
            while (*p)
            {
                if (*p >= '0' && *p <= '9') has_digit = true;
                else if (*p == '.' || *p == ',') has_dot = true;
                else if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z')) has_alpha = true;
                else if (*p == 'e' || *p == 'E') has_dot = true; // treat exponent as float
                p++;
            }
            if (has_alpha)
            {
                any_non_numeric = true;
                break;
            }
            if (has_dot)
                any_float = true;
            else if (!has_digit)
            {
                any_non_numeric = true;
                break;
            }
        }
        if (any_non_numeric)
            db->cols[j].format = TYPE_STRING;
        else if (any_float)
            db->cols[j].format = TYPE_FLOAT;
        else
            db->cols[j].format = TYPE_INT;
    }
}

int	db_load_from_csv(t_database *db, const char *filename)
{
    int		fd;
    int		rc;

    ft_assert(db && filename);
    fd = open_read_fd(filename);
    if (fd < 0)
    {
        if (filename)
            printf("Error: Could not open file '%s'\n", filename);
        else
            printf("Error: Could not open file '(null)'\n");
        return (-1);
    }
    rc = process_file(fd, db);
    close(fd);
    /* after loading, infer column types so alignment rules apply */
    infer_column_types(db);
    if (filename)
        printf("Loaded %zu rows from '%s'\n", db->nrows, filename);
    else
        printf("Loaded %zu rows from '(null)'\n", db->nrows);
    return (rc);
}
