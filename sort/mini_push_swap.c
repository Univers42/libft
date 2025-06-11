#include "ft_sort.h"

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

// Simulate push: move top of src to top of dest
void push(int *src, int *src_size, int *dest, int *dest_size) {
    if (*src_size == 0) return;
    for (int i = *dest_size; i > 0; i--)
        dest[i] = dest[i - 1];
    dest[0] = src[0];
    (*dest_size)++;
    for (int i = 0; i < *src_size - 1; i++)
        src[i] = src[i + 1];
    (*src_size)--;
}

// Simulate minimal push_swap hybrid using two stacks and chunking
void mini_pushswap_hybrid(int *arr, int size, int *tag_to_value) {
    tag_array(arr, size, tag_to_value);

    int *a = malloc(size * sizeof(int));
    int *b = malloc(size * sizeof(int));
    int a_size = size, b_size = 0;
    memcpy(a, arr, size * sizeof(int));

    int chunk_count = (size <= 100) ? 5 : 11;
    int chunk_size = (size + chunk_count - 1) / chunk_count;

    // Push elements chunk by chunk from A to B
    for (int c = 0; c < chunk_count; c++) {
        int min_tag = c * chunk_size;
        int max_tag = (c + 1) * chunk_size;
        if (max_tag > size) max_tag = size;
        int i = 0;
        while (i < a_size) {
            if (a[i] >= min_tag && a[i] < max_tag) {
                // Bring to top and push to B
                int tag = a[i];
                // Rotate to top (simulate with swaps)
                while (i > 0) {
                    int tmp = a[i];
                    a[i] = a[i - 1];
                    a[i - 1] = tmp;
                    i--;
                }
                push(a, &a_size, b, &b_size);
                i = 0; // Restart scan
            } else {
                i++;
            }
        }
    }

    // Pop back from B to A (sorted order)
    while (b_size > 0) {
    // Find index of min in B
    int min_idx = 0;
    for (int i = 1; i < b_size; i++)
        if (b[i] < b[min_idx])
            min_idx = i;
    // Bring min to top
    while (min_idx > 0) {
        int tmp = b[min_idx];
        b[min_idx] = b[min_idx - 1];
        b[min_idx - 1] = tmp;
        min_idx--;
    }
    push(b, &b_size, a, &a_size);
}
for (int i = 0; i < a_size / 2; i++) {
    int tmp = a[i];
    a[i] = a[a_size - 1 - i];
    a[a_size - 1 - i] = tmp;
}
    // Copy back to arr
    for (int i = 0; i < size; i++)
        arr[i] = a[i];

    free(a);
    free(b);
}

// Test
void print_array(int *arr, int size, const char *label, int *tag_to_value) {
    printf("%s: ", label);
    for (int i = 0; i < size; i++)
        printf("%d ", tag_to_value ? tag_to_value[arr[i]] : arr[i]);
    printf("\n");
}

int main(void) {
    int arr[] = {42, 5, 17, 8, 99, 23, 1, 77, 50, 12};
    int size = sizeof(arr) / sizeof(arr[0]);
    int tag_to_value[size];

    // Print original values
    print_array(arr, size, "Original", NULL);

    // Sort using hybrid
    mini_pushswap_hybrid(arr, size, tag_to_value);

    // Print sorted original values (map tags back)
    print_array(arr, size, "Mini pushswap hybrid sorted", tag_to_value);

    return 0;
}