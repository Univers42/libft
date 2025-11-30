/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_string.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:21:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:40:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYN_STRING_H
# define DYN_STRING_H

# include <stdbool.h>
# include <stdlib.h>
# include "ft_stdlib.h"
# include "ft_string.h"
# include "ft_debug.h"

typedef struct s_dyn_str
{
	size_t	cap;
	size_t	len;
	char	*buff;
}	t_dyn_str;

int		dyn_str_init(t_dyn_str *ret);
int		dyn_str_double(t_dyn_str *v);
int		dyn_str_push(t_dyn_str *v, char el);
char	dyn_str_pop(t_dyn_str *v);
int		dyn_str_idx(t_dyn_str *v, size_t idx);
void	dyn_str_clear(t_dyn_str *v);
int		dyn_str_pushstr(t_dyn_str *v, char *str);
int		dyn_str_pushnstr(t_dyn_str *v, char *str, int len);
void	dyn_str_init_alloc(t_dyn_str *v);
bool	dyn_str_ends_with_str(t_dyn_str *s, char *s2);

#endif