/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_tgetent3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 23:51:29 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/25 23:54:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termcap.h"

char	*fallback_entry(const char *name)
{
	if (!name)
		return (NULL);
	if (strcmp(name, "xterm") == 0 || strstr(name, "xterm-256color"))
		return ("xterm|xterm-256color:am:bs:co#80:li#24:"
			"cl=\\E[H\\E[2J:ce=\\E[K:cd=\\E[J:"
			"cm=\\E[%i%2;%2H:nd=\\E[C:up=\\E[A:le=\\E[D:"
			"ho=\\E[H:so=\\E[7m:se=\\E[27m:md=\\E[1m:"
			"me=\\E[0m:us=\\E[4m:ue=\\E[24m:");
	if (strcmp(name, "vt100") == 0)
		return ("vt100|vt100:am:bs:co#80:li#24:"
			"cl=\\E[H\\E[2J:cm=\\E[%i%2;%2H:nd=\\E[C:"
			"up=\\E[A:le=\\E[D:ho=\\E[H:");
	return (NULL);
}

int	apply_fallback(char *bp, const char *name)
{
	char	*fb;
	char	*dst;
	size_t	len;

	fb = fallback_entry(name);
	if (!fb)
		return (0);
	len = strlen(fb);
	if (!bp)
	{
		dst = (char *)xmalloc(len + 1);
		strcpy(dst, fb);
		access_glob()->term_entry = dst;
	}
	else
	{
		strncpy(bp, fb, len);
		bp[len] = '\0';
		access_glob()->term_entry = bp;
	}
	termcap_mark_fallback();
	return (1);
}

/* small initializer for ctx */
void	init_tgetent_ctx(t_tgetent_ctx *ctx, char *bp)
{
	*ctx = (t_tgetent_ctx){bp, NULL, 0, NULL, NULL};
}
