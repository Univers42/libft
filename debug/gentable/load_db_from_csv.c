#include "database.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ft_debug.h"
#include "ft_stddef.h"
#include <ctype.h>

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

static char *strip_outer_quotes(const char *s)
{
    size_t len;
    char *tmp;

    if (!s)
        return NULL;
    len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"')
    {
        tmp = ft_strdup(s + 1);
        if (!tmp) return NULL;
        tmp[len - 2] = '\0';
        /* collapse doubled quotes -> single quote */
        char *r = tmp;
        char *w = tmp;
        while (*r)
        {
            if (r[0] == '"' && r[1] == '"')
            {
                *w++ = '"';
                r += 2;
                continue;
            }
            *w++ = *r++;
        }
        *w = '\0';
        return tmp;
    }
    return ft_strdup(s);
}

static void classify_columns_from_row(char **fields, size_t field_count, t_database *db)
{
    size_t j;

    if (!db || !fields)
        return;
    int offset = 0;
    if (db->config.auto_increment_id && db->ncols == field_count + 1 && db->cols[0].name && strcasecmp(db->cols[0].name, "id") == 0)
        offset = 1;

    for (j = 0; j < field_count; ++j)
    {
        char *src = strip_outer_quotes(fields[j]);
        if (!src)
        {
            db->cols[j + offset].format = TYPE_STRING;
            continue;
        }
        /* trim spaces */
        char *p = src;
        while (*p && isspace((unsigned char)*p)) p++;
        char *end = src + strlen(src) - 1;
        while (end > p && isspace((unsigned char)*end)) *end-- = '\0';

        if (*p == '\0')
        {
            db->cols[j + offset].format = TYPE_STRING;
            free(src);
            continue;
        }

        /* normalize numbers: handle thousands separators and comma vs dot decimals */
        int commas = 0, dots = 0;
        for (char *q = p; *q; ++q)
        {
            if (*q == ',') commas++;
            else if (*q == '.') dots++;
        }
        char *norm = NULL;
        if (commas > 0 && dots == 0)
        {
            /* could be either thousand separators or comma decimal; if single comma, assume decimal */
            if (commas == 1)
            {
                norm = malloc(strlen(p) + 1);
                if (norm) {
                    strcpy(norm, p);
                    for (char *q = norm; *q; ++q) if (*q == ',') *q = '.';
                }
            }
            else
            {
                /* multiple commas: treat as thousand separators -> remove commas */
                norm = malloc(strlen(p) + 1);
                if (norm) {
                    char *w = norm;
                    for (char *q = p; *q; ++q) if (*q != ',') *w++ = *q;
                    *w = '\0';
                }
            }
        }
        else if (dots > 0 && commas > 0)
        {
            /* mixed: decide by last occurrence */
            char *last_dot = strrchr(p, '.');
            char *last_comma = strrchr(p, ',');
            if (last_dot && last_comma)
            {
                if (last_dot > last_comma)
                {
                    /* dot decimal, remove commas */
                    norm = malloc(strlen(p) + 1);
                    if (norm) {
                        char *w = norm;
                        for (char *q = p; *q; ++q) if (*q != ',') *w++ = *q;
                        *w = '\0';
                    }
                }
                else
                {
                    /* comma decimal: replace last comma with dot and remove other commas */
                    norm = strdup(p);
                    if (norm) {
                        char *last = strrchr(norm, ',');
                        if (last) *last = '.';
                        char *w = norm;
                        for (char *q = norm; *q; ++q) if (*q != ',') *w++ = *q;
                        *w = '\0';
                    }
                }
            }
        }
        else
        {
            norm = strdup(p);
        }

        bool is_numeric = false;
        if (norm)
        {
            char *endptr = NULL;
            errno = 0;
            double v = strtod(norm, &endptr);
            (void)v;
            if (endptr && *endptr == '\0' && errno == 0)
            {
                /* decide int vs float */
                if (strchr(norm, '.') || strchr(norm, 'e') || strchr(norm, 'E'))
                    db->cols[j + offset].format = TYPE_FLOAT;
                else
                    db->cols[j + offset].format = TYPE_INT;
                is_numeric = true;
            }
            else
            {
                db->cols[j + offset].format = TYPE_STRING;
            }
            free(norm);
        }
        else
        {
            db->cols[j + offset].format = TYPE_STRING;
        }
        free(src);
    }
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
		/* on the very first data row, classify column formats early so alignment applies */
		if (db->nrows == 0)
			classify_columns_from_row(fields, field_count, db);
		/* handle possible prepending of id placeholder */
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
			new_fields[0] = ft_strdup("");
			for (j = 0; j < field_count; ++j)
				new_fields[j + 1] = fields[j];
			new_fields[field_count + 1] = NULL;
			db_add_row_with_label(db, NULL, (const char **)new_fields, field_count + 1);
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

static const char *format_name(int fmt)
{
    if (fmt == TYPE_INT) return "INT";
    if (fmt == TYPE_FLOAT) return "FLOAT";
    return "STRING";
}

static void print_column_formats(t_database *db)
{
    size_t j;
    if (!db) return;
    printf("Detected column formats:\n");
    for (j = 0; j < db->ncols; ++j)
    {
        const char *name = db->cols[j].name ? db->cols[j].name : "(null)";
        printf("  %zu: %s -> %s\n", j, name, format_name(db->cols[j].format));
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
    /* debug: print detected formats */
    print_column_formats(db);
    if (filename)
        printf("Loaded %zu rows from '%s'\n", db->nrows, filename);
    else
        printf("Loaded %zu rows from '(null)'\n", db->nrows);
    return (rc);
}
