/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_string.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:21:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/18 16:36:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYN_STRING_H
# define DYN_STRING_H

# include <stdbool.h>
# include <stdlib.h>
# include "ft_stdlib.h"
# include "ft_string.h"
# include "ft_debug.h"
# include "ft_ctype.h"
# include "ft_stddef.h"
#include <unistd.h>
typedef struct s_dyn_str
{
	size_t	cap;
	size_t	len;
	size_t	pos;
	char	*buff;
}	t_dyn_str;

int		dyn_str_init(void);
int		dyn_str_double(void);
int		dyn_str_push(char el);
int		dyn_str_idx(size_t idx);
int		dyn_str_pushnstr(const char *s, size_t n);
int		dyn_str_pushstr(const char *str);
int		dyn_str_ends_with_str(const char *s);
bool	dyn_str_check_at(size_t pos, ctype_fn fn, char *cmp, size_t len);
char	dyn_str_pop(void);
void	dyn_str_clear(void);
void	dyn_str_init_alloc(void);
void	dyn_str_append_fd(int fd);
void	dyn_str_uppdate_len(size_t val);

#endif
