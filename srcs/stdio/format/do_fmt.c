/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_fmt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 16:03:06 by syzygy            #+#    #+#             */
/*   Updated: 2026/01/11 00:12:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "format.h"
# include "../output/output.h"

//void	do_format(t_out *dest, const char *f, va_list ap)
//{
//	t_stack_mark	smark;
//	char			*s;
//	int				len;
//	int				olen;
//
//	set_stack_mark(&smark);
//	s = dest->nextc;
//	olen = dest->end - dest->nextc;
//	len = ft_vasprintf(&s, olen, f, ap);
//	if (olen >= len)
//	{
//		dest->nextc += len;
//		pop_stack_mark(&smark);
//		return ;
//	}
//	outmem(s, len, dest);
//	pop_stack_mark(&smark);
//}
