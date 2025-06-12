/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/01/15 10:00:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

int	get_min(int *arr, int size)
{
	int	min_val;
	int	i;

	if (!arr || size <= 0)
		return (0);
	min_val = arr[0];
	i = 1;
	while (i < size)
	{
		if (arr[i] < min_val)
			min_val = arr[i];
		i++;
	}
	return (min_val);
}

int	get_max(int *arr, int size)
{
	int	max_val;
	int	i;

	if (!arr || size <= 0)
		return (0);
	max_val = arr[0];
	i = 1;
	while (i < size)
	{
		if (arr[i] > max_val)
			max_val = arr[i];
		i++;
	}
	return (max_val);
}

void	handle_duplicates(int *arr, int *pos, int item)
{
	if (!arr || !pos)
		return ;
	while (*pos < 10000 && item == arr[*pos]) // Add bounds check
		(*pos)++;
}

int	find_min_index(t_stack *stack)
{
	int	min;
	int	idx;
	int	i;

	if (!stack || !stack->data || stack->size <= 0)
		return (-1);
	min = stack->data[0];
	idx = 0;
	i = 1;
	while (i < stack->size)
	{
		if (stack->data[i] < min)
		{
			min = stack->data[i];
			idx = i;
		}
		i++;
	}
	return (idx);
}

int	cmp_int(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}
