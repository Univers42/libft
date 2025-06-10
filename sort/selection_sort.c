/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selection_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 21:58:12 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 13:51:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

static void	ft_swap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	ft_selection_sort(int *arr, int n)
{
	int	i;
	int	j;
	int	min_idx;

	i = 0;
	while (i < n - 1)
	{
		min_idx = i;
		j = i + 1;
		while (j < n)
		{
			if (arr[j] < arr[min_idx])
				min_idx = j;
			j++;
		}
		if (min_idx != i)
			ft_swap(&arr[min_idx], &arr[i]);
		i++;
	}
}
