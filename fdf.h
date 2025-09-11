/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:47:43 by dlesieur          #+#    #+#             */
/*   Updated: 2025/09/11 23:57:19 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
#define FDF_H
#include "libft.h"
#include "camera.h"
#include "map.h"
#include "err_handler.h"

// Forward declaration for t_map if not included by map.h
#ifndef T_MAP_DEFINED
typedef struct s_map t_map;
#define T_MAP_DEFINED
#endif

typedef struct s_fdf
{
	t_handler err;      // Error handler integrated into the structure
	t_window *window;   // Main rendering window
	t_camera *view;     // Camera/view transformation
	t_line *line;       // Line rendering system
	t_map *map;         // Map/wireframe data
	t_arena *arena;
} t_app;

#endif