/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ksort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:17:53 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 22:17:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

// Swap helper
static void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Min-heap helpers
static void heapify_down(int *heap, int size, int i) {
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && heap[l] < heap[smallest])
        smallest = l;
    if (r < size && heap[r] < heap[smallest])
        smallest = r;
    if (smallest != i) {
        swap(&heap[i], &heap[smallest]);
        heapify_down(heap, size, smallest);
    }
}

static void heapify_up(int *heap, int i) {
    if (i && heap[(i - 1) / 2] > heap[i]) {
        swap(&heap[i], &heap[(i - 1) / 2]);
        heapify_up(heap, (i - 1) / 2);
    }
}

static void heap_push(int *heap, int *size, int val) {
    heap[(*size)++] = val;
    heapify_up(heap, *size - 1);
}

static int heap_pop(int *heap, int *size) {
    int ret = heap[0];
    heap[0] = heap[--(*size)];
    heapify_down(heap, *size, 0);
    return ret;
}

// ksort implementation
void ksort(int *arr, int n, int k) {
    if (k <= 0) return; // Already sorted

    int heap_size = 0;
    int *heap = malloc((k + 1) * sizeof(int));
    int i, j = 0;

    // Build initial heap from first k+1 elements
    for (i = 0; i < k + 1 && i < n; i++)
        heap_push(heap, &heap_size, arr[i]);

    // Process the rest of the array
    for (; i < n; i++) {
        arr[j++] = heap_pop(heap, &heap_size);
        heap_push(heap, &heap_size, arr[i]);
    }

    // Extract remaining elements
    while (heap_size > 0)
        arr[j++] = heap_pop(heap, &heap_size);

    free(heap);
}

// Test function
void print_array(int *arr, int size, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("\n");
}

int main(void) {
    int arr[] = {6, 5, 3, 2, 8, 10, 9};
    int size = sizeof(arr) / sizeof(arr[0]);
    int k = 3;

    printf("=== K-Sort Test (k=%d) ===\n", k);
    print_array(arr, size, "Before sorting");
    ksort(arr, size, k);
    print_array(arr, size, "After sorting");

    return 0;
}