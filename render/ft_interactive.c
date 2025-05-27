/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_interactive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/27 19:01:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

void	ft_print_menu(char **options, int count, int selected)
{
	int i = 0;
	
	ft_printf("\n%s┌─ SELECT OPTION ─┐%s\n", CYAN, RESET);
	while (i < count)
	{
		if (i == selected)
			ft_printf("%s│ ► %-12s │%s\n", GREEN, options[i], RESET);
		else
			ft_printf("%s│   %-12s │%s\n", CYAN, options[i], RESET);
		i++;
	}
	ft_printf("%s└─────────────────┘%s\n", CYAN, RESET);
}

void	ft_print_graph(int *values, int count, int max_height)
{
	int max_val = 0;
	int i = 0;
	
	// Find maximum value
	while (i < count)
	{
		if (values[i] > max_val)
			max_val = values[i];
		i++;
	}
	
	// Print graph from top to bottom
	int row = max_height;
	while (row > 0)
	{
		i = 0;
		while (i < count)
		{
			int bar_height = (values[i] * max_height) / max_val;
			if (bar_height >= row)
				ft_printf("%s█%s", GREEN, RESET);
			else
				ft_printf(" ");
			i++;
		}
		ft_printf("\n");
		row--;
	}
	
	// Print values at bottom
	i = 0;
	while (i < count)
	{
		ft_printf("%d", values[i] % 10);
		i++;
	}
	ft_printf("\n");
}

void	ft_print_loading_spinner(int duration_ms)
{
	char *spinner = "|/-\\";
	int i = 0;
	int cycles = duration_ms / 100;
	
	while (i < cycles)
	{
		ft_printf("\r%sLoading %c%s", YELLOW, spinner[i % 4], RESET);
		usleep(100000);
		i++;
	}
	ft_printf("\r%sLoading complete!%s\n", GREEN, RESET);
}

void	ft_print_timeline(char **events, char **times, int count)
{
	int i = 0;
	
	ft_printf("\n%s── TIMELINE ──%s\n", CYAN, RESET);
	while (i < count)
	{
		if (i == count - 1)
			ft_printf("%s└─ %s%s %s%s\n", CYAN, times[i], RESET, events[i], RESET);
		else
			ft_printf("%s├─ %s%s %s%s\n", CYAN, times[i], RESET, events[i], RESET);
		if (i < count - 1)
			ft_printf("%s│%s\n", CYAN, RESET);
		i++;
	}
}
