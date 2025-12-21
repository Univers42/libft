/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrap_combining.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:34:11 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:34:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"
#include "ipc.h"
#include "var.h"
#include "ipc.h"
#include "lexer.h"
#include <ctype.h> 
#include "ft_wctype.h" 
#include <signal.h> 
#include "trap.h"
#include "ds.h"
#include <stdint.h>
#include <unistd.h>

int		contains_marker(const char *s);
size_t	count_combining(const char *s, size_t len);

static void	push_combining(const char *s, size_t i, size_t r, t_dyn_str *ds)
{
	size_t	k;

	dyn_str_push(ds, '\001');
	k = -1;
	while (++k < r)
		dyn_str_push(ds, s[i + k]);
	dyn_str_push(ds, '\002');
}

static void	push_regular(const char *s, size_t i, size_t r, t_dyn_str *ds)
{
	size_t	k;

	k = -1;
	while (++k < r)
		dyn_str_push(ds, s[i + k]);
}

static void	wrap_combining_loop(const char *s, size_t len, t_dyn_str *ds)
{
	mbstate_t	st;
	wchar_t		wc;
	size_t		i;
	size_t		r;

	i = 0;
	ft_memset(&st, 0, sizeof(st));
	while (i < len)
	{
		r = ft_mbrtowc(&wc, s + i, MB_CUR_MAX, &st);
		if (r == (size_t) - 1 || r == (size_t) - 2 || r == 0)
		{
			dyn_str_push(ds, s[i++]);
			ft_memset(&st, 0, sizeof(st));
			continue ;
		}
		if (ft_wcwidth(wc) == 0)
			push_combining(s, i, r, ds);
		else
			push_regular(s, i, r, ds);
		i += r;
	}
	dyn_str_push(ds, '\0');
}

static char	*alloc_and_wrap_combining(const char *s, size_t len,
				size_t comb_count)
{
	t_dyn_str	ds;

	dyn_str_init(&ds);
	ds.cap = len + comb_count * 2 + 1;
	wrap_combining_loop(s, len, &ds);
	return (ds.buff);
}

char	*wrap_combining(const char *s)
{
	size_t	len;
	size_t	comb_count;

	if (contains_marker(s))
		return (ft_strdup(s));
	len = ft_strlen(s);
	comb_count = count_combining(s, len);
	if (comb_count == 0)
		return (ft_strdup(s));
	return (alloc_and_wrap_combining(s, len, comb_count));
}
