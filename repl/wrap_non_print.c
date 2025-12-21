/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrap_non_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:34:36 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:34:36 by marvin           ###   ########.fr       */
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

/* Wrap ANSI CSI/escape sequences with readline non-printing markers \001...\002
   If the prompt already contains \001 markers, do nothing. */
static int	contains_marker(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\001')
			return (1);
		++i;
	}
	return (0);
}

static size_t	count_esc(const char *s, size_t len, int *need_wrap)
{
	size_t	i;
	size_t	esc_count;
	size_t	j;

	i = 0;
	esc_count = 0;
	*need_wrap = 0;
	while (i < len)
	{
		if (s[i] == '\033')
		{
			*need_wrap = 1;
			++esc_count;
			if (i + 1 < len && s[i + 1] == '[')
			{
				j = i + 2;
				while (j < len && (unsigned char)s[j] < 0x40)
					++j;
				if (j < len)
					i = j;
			}
		}
		++i;
	}
	return (esc_count);
}

static size_t	handle_esc(const char *s, size_t len, size_t i, t_dyn_str *ds)
{
	dyn_str_push(ds, '\001');
	dyn_str_push(ds, s[i++]);
	if (i < len && s[i] == '[')
	{
		dyn_str_push(ds, s[i++]);
		while (i < len)
		{
			dyn_str_push(ds, s[i]);
			if ((unsigned char)s[i] >= 0x40 && (unsigned char)s[i] <= 0x7E)
			{
				++i;
				break ;
			}
			++i;
		}
	}
	else if (i < len)
		dyn_str_push(ds, s[i++]);
	dyn_str_push(ds, '\002');
	return (i);
}

char	*build_wrapped(const char *s, size_t len, size_t esc_count)
{
	t_dyn_str	ds;
	size_t		i;
	size_t		init_cap;

	i = 0;
	init_cap = len + esc_count * 2 + 1;
	ds.cap = init_cap;
	ds.len = 0;
	ds.pos = 0;
	ds.data = NULL;
	ds.buff = malloc(init_cap);
	if (!ds.buff)
		return (ft_strdup(s));
	while (i < len)
	{
		if (s[i] == '\033')
			i = handle_esc(s, len, i, &ds);
		else
			dyn_str_push(&ds, s[i++]);
	}
	dyn_str_push(&ds, '\0');
	return (ds.buff);
}

char	*wrap_nonprint(const char *s)
{
	size_t	len;
	int		need_wrap;
	size_t	esc_count;

	if (!s)
		return (NULL);
	if (contains_marker(s))
		return (ft_strdup(s));
	len = ft_strlen(s);
	esc_count = count_esc(s, len, &need_wrap);
	if (!need_wrap)
		return (ft_strdup(s));
	return (build_wrapped(s, len, esc_count));
}
