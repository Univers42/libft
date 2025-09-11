/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   powerset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 01:23:19 by syzygy            #+#    #+#             */
/*   Updated: 2025/09/12 01:44:50 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

void print_subset(int *arr, int *subset, int subset_size)
{
    int i;
    printf("{ ");
    for (i = 0; i < subset_size; i++)
        printf("%d ", subset[i]);
    printf("}\n");
}

void backtrack(int *arr, int n, int idx, int *subset, int subset_size, int sum, int target, int *found)
{
    if (idx == n)
    {
        if (subset_size > 0 && sum == target)
        {
            print_subset(arr, subset, subset_size);
            *found = 1;
        }
        return;
    }
    // Include arr[idx]
    subset[subset_size] = arr[idx];
    backtrack(arr, n, idx + 1, subset, subset_size + 1, sum + arr[idx], target, found);
    // Exclude arr[idx]
    backtrack(arr, n, idx + 1, subset, subset_size, sum, target, found);
}

void powerset_sum(int *arr, int n, int target)
{
    int subset[n];
    int found = 0;
    backtrack(arr, n, 0, subset, 0, 0, target, &found);
    if (!found)
        printf("No subset sums to %d\n", target);
}

int main(int argc, char **argv)
{
    int *arr;
    int n, i, target;

    if (argc < 3)
    {
        write(2, "Usage: ./powerset <target> <list of integers>\n", 47);
        return (1);
    }
    target = atoi(argv[1]);
    n = argc - 2;
    arr = malloc(sizeof(int) * n);
    if (!arr)
    {
        write(2, "Memory allocation failed\n", 25);
        return (1);
    }
    for (i = 0; i < n; i++)
        arr[i] = atoi(argv[i + 2]);
    powerset_sum(arr, n, target);
    free(arr);
    return (0);
}