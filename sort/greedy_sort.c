/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greedy_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:26:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/15 18:14:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

void	greedy_sort(int *arr, int size)
{
	t_stck	a;
	t_stck	b;

	if (!arr || size <= 1)
		return ;
	initialize_stacks(&a, &b, arr, size);
	if (!a.data || !b.data)
	{
		free(a.data);
		free(b.data);
		return ;
	}
	greedy_sort_algorithm(&a, &b);
	copy_result_back(arr, &a, size);
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