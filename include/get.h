/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:27:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/09 17:35:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_H
# define GET_H

#include "libft.h"

typedef struct s_get_singletons
{
	t_alias *alias;
	t_alias_state *state;
	t_arena         *arena;
	t_arr			*arr;
	t_pos			pos;
	t_vec2			vec2d;
	t_position		fpoint3;
	t_sphere_data	sphere;
	t_parse			parse;
	t_line2			line2d;
	t_line3			line3d;
	
}t_get_singletons;

// static  void init_all_singletons()
// {

// }

// static inline void destroy_singletons()
// {

// }

// static inline t_get_singeletons get_singletons()
// {
	
// }

# endif