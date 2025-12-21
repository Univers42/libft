/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_wrap_comb.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 02:29:51 by marvin            #+#    #+#             */
/*   Updated: 2025/12/21 02:29:51 by marvin           ###   ########.fr       */
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


int	contains_marker(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\001')
			return 1;
		++i;
	}
	return 0;
}

size_t	count_combining(const char *s, size_t len)
{
	size_t		i;
	size_t		comb_count;
	mbstate_t	st;
	wchar_t		wc;
	size_t		r;

	comb_count = 0;
	i = 0;
	ft_memset(&st, 0, sizeof(st));
	while (i < len)
	{
		r = ft_mbrtowc(&wc, s + i, MB_CUR_MAX, &st);
		if (r == (size_t)-1 || r == (size_t)-2 || r == 0)
		{
			i++;
			ft_memset(&st, 0, sizeof(st));
			continue ;
		}
		if (ft_wcwidth(wc) == 0)
			comb_count++;
		i += r;
	}
	return (comb_count);
}