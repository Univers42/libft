/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:59:18 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/10 22:25:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Provide the singleton instance */
t_input	*get_input(void)
{
	static t_input	st = {0};
	return (&st);
}

/*
** input_init - initialize top-level parsefile and stdin_state.
** Uses ft_lseek instead of lseek and keeps behaviour simple.
*/
void input_init(void)
{
	t_input	*in;
	int		istty;
	off_t	pos;

	in = get_input();
	/* prepare base buffer if not present */
	if (in->basepf.buf == NULL)
		in->basepf.buf = (char *)malloc(BUFSIZ + PUNGETC_MAX + 1);
	if (in->basepf.buf)
	{
		in->basepf.nextc = in->basepf.buf;
		in->basepf.nleft = 0;
	}
	in->basepf.linno = 1;
	in->basepf.fd = -1;
	in->basepf.eof = 0;
	in->parsefile = &in->basepf;
	istty = 0;
	if (tcgetattr(0, &in->stdin_state.tios)== 0)
	istty = 1;
	in->stdin_state.pip[0] = 0;
	in->stdin_state.pip[1] = 0;
	in->stdin_state.pending = 0;
	in->stdin_istty = istty;
	if (istty)
	{
		in->stdin_state.seekable = 0;
		in->stdin_state.canon = in->stdin_state.tios.c_lflag & ICANON;
	}
	else
	{
		pos = lseek(0, 0, SEEK_CUR);
		in->stdin_state.seekable = (pos >= 0) ? 1 : 0;
		in->stdin_state.canon = 0;
	}
}
