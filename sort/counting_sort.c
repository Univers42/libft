/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   counting_sort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:46:03 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 02:59:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"


void    counting_sort(int *arr, int size)
{
    int *count;
    int min;
    int max;
    int range;
    int i;
    int j;

    if (!arr || size <= 1)
        return ;
    min = get_min(arr, size);
    max = get_max(arr, size);
    range = max - min + 1;
    count = calloc(range, sizeof(int));
    if (!count)
        return ;
    i = 0;
    while (i < size)
    {
        count[arr[i] - min]++;
        i++;
    }
    i = 0;
    j = 0;
    while (i < range)
    {
        while (count[i] > 0)
        {
            arr[j++] = i + min;
            count[i]--;
        }
        i++;
    }
    free(count);
}

//int	main(void)
//{
//    int	arr[] = {4, 2, 2, 8, 3, 3, 1};
//    int	size = sizeof(arr) / sizeof(arr[0]);
//    int	i;
//
//    printf("Before sorting:\n");
//    i = 0;
//    while (i < size)
//    {
//        printf("%d ", arr[i]);
//        i++;
//    }
//    printf("\n");
//    collection_sort(arr, size);
//    printf("After sorting:\n");
//    i = 0;
//    while (i < size)
//    {
//        printf("%d ", arr[i]);
//        i++;
//    }
//    printf("\n");
//    return (0);
//}