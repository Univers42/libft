/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_table.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/09/04 17:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Every column used to be a hardcoded 12 characters wide, with the rule
** drawn as a fixed 14 dashes. Any cell longer than that overflowed and left
** the rule out of step with the rows, and coloured cells drifted further
** still because the escape bytes were counted as width. Columns are now
** measured from the widest value they actually hold, using ft_vislen() so
** that colour costs nothing.
*/

#include "ft_render.h"
#include "ft_stdio.h"
#include "ft_stdlib.h"
#include "xalloc.h"

static void	measure_row(int *widths, char **row, int cols)
{
	int	i;
	int	len;

	if (!row)
		return ;
	i = 0;
	while (i < cols)
	{
		if (row[i])
		{
			len = (int)ft_vislen(row[i]);
			if (len > widths[i])
				widths[i] = len;
		}
		i++;
	}
}

static int	*measure_columns(char **headers, char ***data, int rows, int cols)
{
	int	*widths;
	int	r;

	widths = (int *)ft_calloc((size_t)cols, sizeof(int));
	if (!widths)
		return (NULL);
	measure_row(widths, headers, cols);
	r = 0;
	while (r < rows)
	{
		measure_row(widths, data[r], cols);
		r++;
	}
	return (widths);
}

static void	print_cell(const char *s, int width)
{
	int	pad;

	if (!s)
		s = "";
	ft_printf(" %s", s);
	pad = width - (int)ft_vislen(s);
	while (pad-- > 0)
		ft_printf(" ");
	ft_printf(" │");
}

static void	print_table_row(char **row, const int *widths, int cols)
{
	int	i;

	ft_printf("│");
	i = 0;
	while (i < cols)
	{
		if (row)
			print_cell(row[i], widths[i]);
		else
			print_cell("", widths[i]);
		i++;
	}
	ft_printf("\n");
}

void	ft_print_table(char **headers, char ***data, int rows, int cols)
{
	int	*widths;
	int	i;

	if (!headers || !data || cols <= 0 || rows < 0)
		return ;
	widths = measure_columns(headers, data, rows, cols);
	if (!widths)
		return ;
	print_table_border(widths, cols, "┌", "─");
	print_table_row(headers, widths, cols);
	print_table_border(widths, cols, "├", "─");
	i = 0;
	while (i < rows)
	{
		print_table_row(data[i], widths, cols);
		i++;
	}
	print_table_border(widths, cols, "└", "─");
	fn_free(widths);
}
