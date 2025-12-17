/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flush_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 22:47:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 00:44:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "lseek.h"
#include <unistd.h>
#include <string.h>

/*
** flush_input - ensure the top-level input buffer is cleared.
*/
void flush_input(void)
{
	t_input *in;
	int left;
	int saved_int;
	char buf[BUFSIZ];
	ssize_t r;
	int to_discard;

	in = get_input();
	left = in->basepf.nleft + input_get_lleft(&in->basepf);
	intoff();
	saved_int = error_saveint();
	if (in->stdin_state.seekable && left)
		(void)ft_lseek(0, -left, SEEK_CUR);
	else
	{
		to_discard = 0;
		if (in->stdin_state.pending > left)
			to_discard = in->stdin_state.pending - left;
		while (to_discard > 0)
		{
			r = read(0, buf, (size_t)((to_discard > (int)BUFSIZ) ? BUFSIZ : (size_t)to_discard));
			if (r <= 0)
				break;
			to_discard -= (int)r;
		}
		if (in->stdin_state.pending <= left)
			in->stdin_state.pending = 0;
		else
			in->stdin_state.pending -= left;
	}
	input_set_lleft(&in->basepf, in->basepf.nleft = 0);
	error_restoreint(saved_int);
}
