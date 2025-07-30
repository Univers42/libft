/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 19:52:01 by dlesieur          #+#    #+#             */
/*   Updated: 2025/07/30 20:22:41 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// forward declarations
typedef struct s_parser_base t_parser_base;
typedef struct s_parser_vtable  t_parser_vtable;

typedef enum e_parser_type
{
	PARSER_MAP_FDF,
	PARSER_MAP_OBJ,
	PARSER_MAP_CSV,
	PARSER_MAP_CUSTOM
}t_parser_type;

typedef struct s_parser_base
{
	t_parser_vtable	*vtable;
	t_parser_type	type;
	char			*buffer;
	char			*error_message;
	void			*specific_data;
	size_t			buffer_size;
	size_t			buffer_capacity;
	size_t			positiion;
	size_t			position;
	bool			error_state;
}					t_parser_base;

typedef struct s_parser_vtable
{
	//Core parsing methods
	bool	(*parse_chunk)(t_parser_base *parser, const char *data, size_t size);
	bool	(*parse_value)(t_parser_base *parser, const char *data, size_t *offset);
	bool	(*parse_color)(t_parser_base *parser, const char *data, size_t *offset);
	bool	(*finalize)(t_parser_base *parser);

	// Utility methods
	bool	(*is_delimiter)(char c);
	size_t	(*skip_delimiters)(const char *data, size_t size, size_t offset);
	bool	(*line_advance)(t_parser_base *parser);
	void	(*cleanup)(t_parser_base *parser);
	bool	(*realloc_data)(t_parser_base *parser);
	//Validation
	bool	(*validate_format)(t_parser_base *parser, const char *data, size_t size);
}			t_parser_vtable;

typedef struct s_data
{
	float		*points;
	uint32_t	*colors;
	int			width;
	int			height;
	int			x;
	int			y;
	int			z;
	uint32_t	current_color;
	float		min_z;
	float		max_z;
	bool		z_set;
	bool		width_set;
	size_t		values_read;
	size_t		array_capacity;
}				t_ctx_data;

typedef struct s_obj_data
{
	float	*vertices;
	int		*faces;
	float	*normals;
	size_t	vertex_count;
	size_t	face_count;
	size_t	normal_count;
	size_t	vertex_capacity;
	size_t	face_capacity;
}			t_obj_data;

typedef struct s_csv_data
{
    char       **fields;
    char       **headers;
    size_t     row_count;
    size_t     column_count;
    size_t     field_capacity;
    char       delimiter;
    bool       has_headers;
} t_csv_data;

bool	parse_chunk(t_parser_base *parser, const char *data, size_t size);
bool	parse_val(t_parser_base *parser, const char *data, size_t *offset, void *ctx);
bool	parse_color(t_parser_base *parser, const char *data, size_t *offset);
bool	finalize(t_parser_base *parser);
bool	finalize(t_parser_base *parser);
bool	is_delimiter(char c);
bool	realloc_data(t_parser_base *parser);
size_t	skip_delimiters(const char *dat, size_t size, size_t offset);
bool	line_advance(t_parser_base *parser);
void	cleanup(t_parser_base *parser);
bool	validate_format(t_parser_base *parser, const char *data, size_t size);
#endif

