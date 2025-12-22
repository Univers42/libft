/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   database.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 01:03:47 by marvin            #+#    #+#             */
/*   Updated: 2025/12/22 01:03:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_H
# define DATABASE_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <locale.h>
#include "ft_stddef.h"
#include "ft_string.h"
#include "ft_math.h"
#include "ft_wctype.h"
#include "ft_memory.h"
#include "libft.h"
#include "system.h"

/* ensure_locale: call setlocale once if available; otherwise noop */

// Format style configuration
typedef struct	s_format_style
{
    t_align	number_align;
    t_align	string_align;
    t_color	title;
    t_color	header;
    t_color	footer;
    t_color	even_row;
    t_color	odd_row;
    /* background colors */
    t_color	background_header;
    t_color	background_cell;
    t_color	background_body;
    t_color	background_footer;
}	t_format_style;

// Display configuration flags
typedef struct s_display_config
{
    bool	show_title;
    bool	show_header;
    bool	show_footer;
    bool	show_row_numbers;
    bool	show_left_attributes;
    bool	alternating_colors;
    bool	use_bold_header;
    char	col_separator;
    bool	auto_increment_id;
}	t_display_config;

// Column definition
typedef struct s_col
{
    char	*name;
    t_type	format;
    t_align	align;
    size_t	width;
}	t_col;

// Row structure
typedef struct s_row
{
    int     id;
    char	*label;  // Optional row label/attribute
    char	**data;
    size_t	ncols;
}	t_row;

// Database structure
typedef struct s_database
{
    char				*title;
    t_col				*cols;
    size_t				ncols;
    size_t				cols_capacity;
    t_row				*rows;
    size_t				nrows;
    size_t				rows_capacity;
    char				*footer;
    t_format_style		style;
    t_display_config	config;
} t_database;

/* padding/truncation context used by formatting helpers */
typedef struct s_pad_ctx
{
	char		*buf;
	const char	*text;
	size_t		width;
	t_align		align;
	size_t		bufsize;
	int			dlen;
}	t_pad_ctx;

#ifdef __cplusplus
extern "C" {
#endif

void				db_set_style(t_database *db, t_format_style style);
void				db_set_config(t_database *db, t_display_config config);
void				db_set_footer(t_database *db, const char *footer);
void				db_free(t_database *db);
t_format_style		default_style(void);
t_display_config	default_config(void);
void				compute_padding_if_not_too_long(t_pad_ctx *ctx);
void				format_cell(t_pad_ctx *ctx);
void				print_rgb_color(t_color c);
void				print_rgb_bg_color(t_color c);
t_color				make_color(uint8_t r, uint8_t g, uint8_t b);
t_color				make_background(uint8_t r, uint8_t g, uint8_t b);
void				utf8_truncate_by_display_width(const char *src,
						size_t max_display, char *dst, size_t dst_size);
void				print_separator(size_t *widths, size_t ncols, size_t label_width,
                           bool show_label, const char *left, const char *mid, 
                           const char *right, const char *line);
void				db_render(t_database *db);
char				**csv_split(const char *str, char delimiter);
void				free_split(char **arr);
char				*trim_whitespace(char *str);
t_database			*db_create(const char *title);
int					db_add_column(t_database *db, const char *name,
						t_type format, t_align align);
int					db_add_row_with_label(t_database *db, const char *label,
						const char **values, size_t nvalues);
int					db_add_row(t_database *db, ...);
int					db_load_from_csv(t_database *db, const char *filename);

/* management helpers */
int				db_set_cell(t_database *db, size_t row, size_t col, const char *value);
int				db_map_column(t_database *db, size_t col_idx, char (*f)(unsigned int, char));
int				add_value_idx_column(t_database *db, t_col *col, int idx, int val);
int				add_value(t_database *db, int lineno, t_col *col, char *value);
bool				is_id_already_existing(t_database *db, const char *id_value, size_t id_col);
int				db_next_auto_id(t_database *db);
void				calculate_column_widths(t_database *db, size_t *widths, size_t *label_width);
int					display_width(const char *s);
void				utf8_truncate_by_display_width(const char *src, size_t max_display,
						char *dst, size_t dst_size);
void				pad_center(char *buf, const char *text, size_t pad, size_t bufsize);
void				pad_right(char *buf, const char *text, size_t pad, size_t bufsize);
void				pad_left(char *buf, const char *text, size_t pad, size_t bufsize);
void				format_cell(t_pad_ctx *ctx);
void				compute_padding_if_not_too_long(t_pad_ctx *ctx);
#ifdef __cplusplus
}
#endif

#endif
