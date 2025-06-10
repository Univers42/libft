#include <unistd.h>


#include <stdio.h>
#include <stdlib.h>

// Tagging: replace each value with its index in the sorted array
void tag_array(int *arr, int size) {
    int *sorted = malloc(size * sizeof(int));
    for (int i = 0; i < size; i++)
        sorted[i] = arr[i];
    // Simple selection sort for demonstration
    for (int i = 0; i < size - 1; i++)
        for (int j = i + 1; j < size; j++)
            if (sorted[i] > sorted[j]) {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
    // Tag original
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (arr[i] == sorted[j]) {
                arr[i] = j;
                break;
            }
    free(sorted);
}

// Radix sort on tagged array
void radix_sort(int *arr, int size) {
    int max = 0;
    for (int i = 0; i < size; i++)
        if (arr[i] > max)
            max = arr[i];
    int max_bits = 0;
    while ((max >> max_bits) != 0)
        max_bits++;

    int *tmp = malloc(size * sizeof(int));
    for (int bit = 0; bit < max_bits; bit++) {
        int j = 0;
        // Collect zeros
        for (int i = 0; i < size; i++)
            if (((arr[i] >> bit) & 1) == 0)
                tmp[j++] = arr[i];
        // Collect ones
        for (int i = 0; i < size; i++)
            if (((arr[i] >> bit) & 1) == 1)
                tmp[j++] = arr[i];
        // Copy back
        for (int i = 0; i < size; i++)
            arr[i] = tmp[i];
    }
    free(tmp);
}

// Chunking: sort by chunks after tagging (for demonstration, just prints chunks)
void chunk_sort(int *arr, int size, int chunk_count) {
    int chunk_size = size / chunk_count + (size % chunk_count ? 1 : 0);
    printf("Chunking into %d chunks:\n", chunk_count);
    for (int c = 0; c < chunk_count; c++) {
        printf("Chunk %d: ", c + 1);
        for (int i = 0; i < size; i++)
            if (arr[i] / chunk_size == c)
                printf("%d ", arr[i]);
        printf("\n");
    }
}

// Hybrid sort: tag, chunk, then radix sort
void hybrid_sort(int *arr, int size) {
    tag_array(arr, size);
    if (size <= 5) {
        // For small arrays, use insertion sort
        for (int i = 1; i < size; i++) {
            int key = arr[i], j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    } else if (size <= 100) {
        chunk_sort(arr, size, 5); // Example: 5 chunks
        radix_sort(arr, size);
    } else {
        chunk_sort(arr, size, 11); // Example: 11 chunks for large arrays
        radix_sort(arr, size);
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
    print_array(arr, size, "Original");
    hybrid_sort(arr, size);
    print_array(arr, size, "Hybrid sorted (tagged/radix)");
    return 0;
}