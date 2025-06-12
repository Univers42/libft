/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greedy_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:26:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 11:07:52 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

static void	rotate_stack(t_stack *stack)
{
	int	tmp;
	int	i;

	if (stack->size < 2)
		return ;
	tmp = stack->data[0];
	i = 0;
	while (i < stack->size - 1)
	{
		stack->data[i] = stack->data[i + 1];
		i++;
	}
	stack->data[stack->size - 1] = tmp;
}

static void	push_stack(t_stack *src, t_stack *dest)
{
	int	i;

	if (src->size == 0)
		return ;
	i = dest->size;
	while (i > 0)
	{
		dest->data[i] = dest->data[i - 1];
		i--;
	}
	dest->data[0] = src->data[0];
	dest->size++;
	i = 0;
	while (i < src->size - 1)
	{
		src->data[i] = src->data[i + 1];
		i++;
	}
	src->size--;
}

void	greedy_sort(int *arr, int size)
{
	t_stack	a;
	t_stack	b;
	int		min_idx;
	int		i;

	if (!arr || size <= 1)
		return ;
	a.data = malloc(size * sizeof(int));
	a.size = size;
	b.data = malloc(size * sizeof(int));
	b.size = 0;
	if (!a.data || !b.data)
	{
		free(a.data);
		free(b.data);
		return ;
	}
	i = 0;
	while (i < size)
	{
		a.data[i] = arr[i];
		i++;
	}
	while (a.size > 0)
	{
		min_idx = find_min_index(&a);
		while (min_idx > 0)
		{
			rotate_stack(&a);
			min_idx--;
		}
		push_stack(&a, &b);
	}
	while (b.size > 0)
		push_stack(&b, &a);
	i = 0;
	while (i < size)
	{
		arr[i] = a.data[i];
		i++;
	}
	free(a.data);
	free(b.data);
}

// Test
//int main(void) {
//    int arr[] = {5, 2, 4, 3, 1};
//    int size = sizeof(arr) / sizeof(arr[0]);
//    greedy_sort(arr, size);
//    for (int i = 0; i < size; i++)
//        printf("%d ", arr[i]);
//    printf("\n");
//    return 0;
//}