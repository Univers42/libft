/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_table.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 19:03:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	ft_get_max_width(char **headers, char ***data, int rows, int cols, int col_idx)
{
	int max_width = ft_strlen(headers[col_idx]);
	int i = 0;
	
	(void)cols; // Suppress unused parameter warning
	while (i < rows)
	{
		int len = ft_strlen(data[i][col_idx]);
		if (len > max_width)
			max_width = len;
		i++;
	}
	return (max_width);
}

void	ft_print_table(char **headers, char ***data, int rows, int cols)
{
	int *col_widths = malloc(sizeof(int) * cols);
	int i = 0;
	int j;
	
	// Calculate column widths
	while (i < cols)
	{
		col_widths[i] = ft_get_max_width(headers, data, rows, cols, i);
		i++;
	}
	
	// Print top border
	ft_printf("%s+", CYAN);
	i = 0;
	while (i < cols)
	{
		ft_print_padding(col_widths[i] + 2, '-');
		if (i < cols - 1)
			ft_printf("+");
		i++;
	}
	ft_printf("+%s\n", RESET);
	
	// Print headers
	ft_printf("%s|", CYAN);
	i = 0;
	while (i < cols)
	{
		ft_printf(" %s%-*s%s ", BOLD, col_widths[i], headers[i], RESET CYAN);
		if (i < cols - 1)
			ft_printf("|");
		i++;
	}
	ft_printf("|%s\n", RESET);
	
	// Print separator
	ft_printf("%s+", CYAN);
	i = 0;
	while (i < cols)
	{
		ft_print_padding(col_widths[i] + 2, '-');
		if (i < cols - 1)
			ft_printf("+");
		i++;
	}
	ft_printf("+%s\n", RESET);
	
	// Print data rows
	i = 0;
	while (i < rows)
	{
		ft_printf("%s|", CYAN);
		j = 0;
		while (j < cols)
		{
			ft_printf(" %-*s ", col_widths[j], data[i][j]);
			if (j < cols - 1)
				ft_printf("%s|%s", CYAN, RESET);
			j++;
		}
		ft_printf("%s|%s\n", CYAN, RESET);
		i++;
	}
	
	// Print bottom border
	ft_printf("%s+", CYAN);
	i = 0;
	while (i < cols)
	{
		ft_print_padding(col_widths[i] + 2, '-');
		if (i < cols - 1)
			ft_printf("+");
		i++;
	}
	ft_printf("+%s\n", RESET);
	
	free(col_widths);
}

void	ft_print_columns(char **col1, char **col2, int rows)
{
	int i = 0;
	int max_width = 0;
	
	// Find max width of first column
	while (i < rows)
	{
		int len = ft_strlen(col1[i]);
		if (len > max_width)
			max_width = len;
		i++;
	}
	
	i = 0;
	while (i < rows)
	{
		ft_printf("%-*s │ %s\n", max_width, col1[i], col2[i]);
		i++;
	}
}
