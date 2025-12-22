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
		i = -1;
		while (++i < field_count)
		{
			trimmed = trim_whitespace(fields[i]);
			db_add_column(db, trimmed, TYPE_STRING, ALIGN_LEFT);
		}
		*first_line = 0;
	}
	else
		db_add_row_with_label(db, NULL, (const char **)fields, field_count);
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
	if (filename)
		printf("Loaded %zu rows from '%s'\n", db->nrows, filename);
	else
		printf("Loaded %zu rows from '(null)'\n", db->nrows);
	return (rc);
}
