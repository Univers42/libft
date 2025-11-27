/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setvarint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:06:26 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/27 16:41:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "var.h"

#define INTMAX_BUFSIZE 25

extern void	ft_fmtstr(char *buf, int size, const char *fmt, ...);

intmax_t	setvarint(const char *name, intmax_t val, int flags)
{
	char	buf[INTMAX_BUFSIZE];

	ft_fmtstr(buf, INTMAX_BUFSIZE, "%", PRIdMAX, val);
	setvar(name, buf, flags);
	return (val);
}
