/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tag_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 21:50:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 22:00:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct s_tagged_element
{
    int value;
    int tag;
}t_tagged_element;

static t_tagged_element *create_tagged_array(int *arr, int size)
{
    t_tagged_element *tag;
    int i, j, rank;

    tag = (t_tagged_element *)malloc(sizeof(t_tagged_element));
    if (!tag)
        return (NULL);
    i = -1;
    while (++i < size)
    {
        rank = 0;
        j = -1;
        while (++j < size)
        {
            if (arr[j] < arr[i])
                rank++;
        }
        tag[i].value = arr[i];
        tag[i].tag = rank;
    }
    return (tag);
}

static void place_by_tags(int *arr, t_tagged_element *tagged, int size)
{
    int i;

    i = -1;
    while (++i < size)
        arr[tagged[i].tag]= tagged[i].value;
}

void tag_sort(int *arr, int size)
{
    t_tagged_element *tagged;

    tagged = create_tagged_array(arr, size);
    if (!tagged)
        return ;
    place_by_tags(arr, tagged, size);
    free(tagged);
}

void print_array(int *arr, int size, const char *label)
{
    int i;
    
    printf("%s: ", label);
    for (i = 0; i < size; i++)
    {
        printf("%d", arr[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("\n");
}

int main(void)
{
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int size = sizeof(arr) / sizeof(arr[0]);
    
    printf("=== Tag Sort Test ===\n");
    print_array(arr, size, "Before sorting");
    tag_sort(arr, size);
    print_array(arr, size, "After sorting");
    
    return (0);
}