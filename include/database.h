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
#include "math.h"
#include "ft_wctype.h"

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

#endif