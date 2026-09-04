/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dirname.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/04 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 19:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ft_dirname: POSIX dirname(3), same contract as glibc's.
**
** Returns the directory part of a path, modifying the buffer in place and
** returning a pointer into it -- or a pointer to a static "." or "/" for the
** cases where no part of the input is the answer. A single backwards pass:
** step over trailing slashes, step back to the last separator, then step
** over any run of slashes before it so "/usr//lib//" gives "/usr" and not
** "/usr/". Nothing is scanned twice.
**
**   "/usr/lib" -> "/usr"      "/usr/" -> "/"      "usr" -> "."
**   "/"        -> "/"         "."     -> "."      ""    -> "."
**
** POSIX lets a path that starts with exactly two slashes mean something
** different from one slash, and glibc honours it: dirname("//") and
** dirname("//a") are "//", while "///a" collapses to "/". That is kept.
*/

#include "ft_string.h"

static char	g_dot[] = ".";

char	*ft_dirname(char *path)
{
	char	*end;
	int		double_root;

	if (!path || !*path)
		return (g_dot);
	double_root = (path[0] == '/' && path[1] == '/' && path[2] != '/');
	end = path + ft_strlen(path) - 1;
	while (end > path && *end == '/')
		end--;
	while (end > path && *end != '/')
		end--;
	if (end == path)
	{
		if (*path != '/')
			return (g_dot);
		path[1 + double_root] = '\0';
		return (path);
	}
	while (end > path && *end == '/')
		end--;
	if (end == path && double_root)
		end++;
	end[1] = '\0';
	return (path);
}
