/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_string.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 23:21:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:15:08 by dlesieur         ###   ########.fr       */
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
# include <unistd.h>
# include "ft_string.h"

typedef struct s_dyn_str
{
	size_t	cap;
	size_t	len;
	size_t	pos;
	char	*buff;
}	t_dyn_str;

/**
 * NOT THREAD SAFE SIGNLETON PATTERRN USAGE IN CASE SIMPLE  USAGE
 */
int			str_init(t_dyn_str *ret);
int			str_double(void);
int			str_push(char el);
int			str_idx(size_t idx);
int			str_pushnstr(const char *s, size_t n);
int			str_pushstr(const char *str);
int			str_ends_with_str(const char *s);
bool		str_check_at(size_t pos, ft_ctype fn, const char *cmp, size_t len);
char		str_pop(void);
void		str_clear(void);
void		str_init_alloc(void);
void		str_append_fd(int fd);
void		str_uppdate_len(size_t val);
t_dyn_str	*get_dyn_str(t_dyn_str *maybe);
void		str_replace_buff(t_dyn_str *v, char *new_buff, size_t new_cap);
void		str_null_terminate(void);

/**
 * THREAD SAFE IN CASE OF MORE COMPLEX CASE
 */

int     dyn_str_init(t_dyn_str *s);
void    dyn_str_free(t_dyn_str *s);
int		dyn_str_push(t_dyn_str *s, char el);
int     dyn_str_pushnstr(t_dyn_str *s, const char *str, size_t n);
int     dyn_str_pushstr(t_dyn_str *s, const char *str);
char    dyn_str_pop(t_dyn_str *s);
int     dyn_str_double(t_dyn_str *s);
int     dyn_str_idx(t_dyn_str *s, size_t idx);
void    dyn_str_null_terminate(t_dyn_str *s);
void    dyn_str_clear(t_dyn_str *s);
int     dyn_str_ends_with_str(t_dyn_str *s, const char *suffix);
void    dyn_str_replace_buff(t_dyn_str *s, char *new_buff, size_t new_cap);
bool    dyn_str_check_at(t_dyn_str *s, size_t pos, int (*fn)(int), const char *cmp);
void	dyn_str_replace_buff(t_dyn_str *s, char *new_buff, size_t new_cap);
void	dyn_str_append_fd(t_dyn_str *str, int fd);
#endif
