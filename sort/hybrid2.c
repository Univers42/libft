#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Compare function for qsort
static int cmp_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Tagging: replace each value with its index in the sorted array, build tag-to-value mapping
void tag_array(int *arr, int size, int *tag_to_value) {
    int *sorted = malloc(size * sizeof(int));
    memcpy(sorted, arr, size * sizeof(int));
    qsort(sorted, size, sizeof(int), cmp_int);

    for (int i = 0; i < size; i++)
        tag_to_value[i] = sorted[i];

    for (int i = 0; i < size; i++) {
        // Binary search for arr[i] in sorted[]
        int left = 0, right = size - 1, tag = 0;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (sorted[mid] == arr[i]) {
                tag = mid;
                break;
            } else if (sorted[mid] < arr[i]) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        arr[i] = tag;
    }
    free(sorted);
}

// Optimized greedy chunk sort
void greedy_chunk_sort(int *arr, int size, int chunk_count) {
    int chunk_size = (size + chunk_count - 1) / chunk_count;
    int *result = malloc(size * sizeof(int));
    int res_idx = 0;
    int *used = calloc(size, sizeof(int));

    for (int c = 0; c < chunk_count; c++) {
        int min_tag = c * chunk_size;
        int max_tag = (c + 1) * chunk_size;
        if (max_tag > size) max_tag = size;
        for (int tag = min_tag; tag < max_tag; tag++) {
            for (int i = 0; i < size; i++) {
                if (!used[i] && arr[i] == tag) {
                    result[res_idx++] = arr[i];
                    used[i] = 1;
                    break;
                }
            }
        }
    }
    memcpy(arr, result, size * sizeof(int));
    free(result);
    free(used);
}

// Hybrid: tag, chunk, greedy insert
void hybrid2_sort(int *arr, int size, int *tag_to_value) {
    tag_array(arr, size, tag_to_value);
    if (size <= 5) {
        // Insertion sort for small arrays
        for (int i = 1; i < size; i++) {
            int key = arr[i], j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    } else if (size <= 100) {
        greedy_chunk_sort(arr, size, 5); // 5 chunks for demonstration
    } else {
        greedy_chunk_sort(arr, size, 11); // 11 chunks for large arrays
    }
}

// Test
void print_array(int *arr, int size, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(void) {
    int arr[] = {42, 5, 17, 8, 99, 23, 1, 77, 50, 12};
    int size = sizeof(arr) / sizeof(arr[0]);
    int tag_to_value[size];
    print_array(arr, size, "Original");
    hybrid2_sort(arr, size, tag_to_value);

    // Map tags back to original values using tag_to_value
    int sorted[size];
    for (int i = 0; i < size; i++)
        sorted[i] = tag_to_value[arr[i]];

    print_array(sorted, size, "Hybrid2 sorted (original values)");
    return 0;
}