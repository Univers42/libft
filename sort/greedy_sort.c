/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greedy_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:26:11 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 02:59:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"

void rotate(t_stack *stack) {
    if (stack->size < 2) return;
    int tmp = stack->data[0];
    for (int i = 0; i < stack->size - 1; i++)
        stack->data[i] = stack->data[i + 1];
    stack->data[stack->size - 1] = tmp;
}

// Push top of src to dest
void push(t_stack *src, t_stack *dest) {
    if (src->size == 0) return;
    for (int i = dest->size; i > 0; i--)
        dest->data[i] = dest->data[i - 1];
    dest->data[0] = src->data[0];
    dest->size++;
    for (int i = 0; i < src->size - 1; i++)
        src->data[i] = src->data[i + 1];
    src->size--;
}

// Greedy sort: always move the minimal element to B, then back to A
void greedy_sort(int *arr, int size) {
    t_stack a = { malloc(size * sizeof(int)), size };
    t_stack b = { malloc(size * sizeof(int)), 0 };
    for (int i = 0; i < size; i++)
        a.data[i] = arr[i];

    // Move all elements to B in sorted order
    while (a.size > 0) {
        int min_idx = find_min_index(&a);
        // Rotate A until min is on top
        while (min_idx-- > 0)
            rotate(&a);
        push(&a, &b);
    }
    // Move back to A (now sorted)
    while (b.size > 0)
        push(&b, &a);

    // Copy back to arr
    for (int i = 0; i < size; i++)
        arr[i] = a.data[i];

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