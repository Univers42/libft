/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_table_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:10:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 17:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_render.h"
#include "ft_stdio.h"
#include "ft_string.h"

static void	print_border_char(char *corner)
{
	if (ft_strcmp(corner, "┌") == 0)
		ft_printf("┐\n");
	else if (ft_strcmp(corner, "├") == 0)
		ft_printf("┤\n");
	else
		ft_printf("┘\n");
}

static void	print_separator_char(char *corner)
{
	if (ft_strcmp(corner, "┌") == 0)
		ft_printf("┬");
	else if (ft_strcmp(corner, "├") == 0)
		ft_printf("┼");
	else
		ft_printf("┴");
}

/*
** Visible width of a string: how many columns it actually occupies on
** screen. Two things are deliberately not counted:
**
**   - ANSI SGR escapes ("\033[...m"), which are zero-width. Measuring a
**     coloured cell with ft_strlen() counts those bytes and pushes every
**     following column out of alignment;
**   - UTF-8 continuation bytes, so that a multi-byte character such as a
**     box-drawing glyph counts as one column rather than three.
*/
size_t	ft_vislen(const char *s)
{
	size_t	n;

	n = 0;
	if (!s)
		return (0);
	while (*s)
	{
		if (*s == '\033')
		{
			while (*s && *s != 'm')
				s++;
			if (*s)
				s++;
			continue ;
		}
		if ((*s & 0xC0) != 0x80)
			n++;
		s++;
	}
	return (n);
}

/*
** Draws one horizontal rule. Each column is padded to its own measured
** width plus the single space on either side of the cell text, so the rule
** always lines up with the rows above and below it.
*/
void	print_table_border(const int *widths, int cols, char *corner,
		char *line)
{
	int	i;
	int	j;

	if (!widths)
		return ;
	ft_printf("%s", corner);
	i = 0;
	while (i < cols)
	{
		j = 0;
		while (j < widths[i] + 2)
		{
			ft_printf("%s", line);
			j++;
		}
		if (i < cols - 1)
			print_separator_char(corner);
		i++;
	}
	print_border_char(corner);
}
