/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strand_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:02:09 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 02:43:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>


// Helper: merge two sorted arrays into result
void merge(int *result, int *a, int a_size, int *b, int b_size) {
    int i = 0, j = 0, k = 0;
    while (i < a_size && j < b_size) {
        if (a[i] <= b[j])
            result[k++] = a[i++];
        else
            result[k++] = b[j++];
    }
    while (i < a_size)
        result[k++] = a[i++];
    while (j < b_size)
        result[k++] = b[j++];
}

// Helper: extract a strand from arr, mark used, return strand size
int extract_strand(int *arr, int *used, int size, int *strand) {
    int last = -2147483648; // INT_MIN
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (!used[i] && arr[i] >= last) {
            strand[count++] = arr[i];
            last = arr[i];
            used[i] = 1;
        }
    }
    return count;
}

// Main strand sort function
void strand_sort(int *arr, int size) {
    int *used = calloc(size, sizeof(int));
    int *result = malloc(size * sizeof(int));
    int result_size = 0;

    while (result_size < size) {
        int *strand = malloc(size * sizeof(int));
        int strand_size = extract_strand(arr, used, size, strand);

        // Merge strand into result
        int *merged = malloc((result_size + strand_size) * sizeof(int));
        merge(merged, result, result_size, strand, strand_size);

        // Copy merged back to result
        memcpy(result, merged, (result_size + strand_size) * sizeof(int));
        result_size += strand_size;

        free(strand);
        free(merged);
    }

    // Copy sorted result back to arr
    memcpy(arr, result, size * sizeof(int));
    free(result);
    free(used);
}

//int main(void) {
//    int arr[] = {64, 34, 25, 12, 22, 11, 90};
//    int size = sizeof(arr) / sizeof(arr[0]);
//
//    printf("=== Strand Sort Test ===\n");
//    print_array(arr, size);
//    strand_sort(arr, size);
//    print_array(arr, size);
//
//    return 0;
//}