#include "ft_sort.h"

int get_min(int *arr, int size)
{
    int min_val;
    int i;

    min_val = arr[0];
    i = 1;
    while (i < size)
    {
        if (arr[i] < min_val)
            min_val = arr[i];
        i++;
    }
    return (min_val);
}

int get_max(int *arr, int size)
{
    int max_val;
    int i;

    max_val = arr[0];
    i = 1;
    while (i < size)
    {
        if (arr[i] > max_val)
            max_val = arr[i];
        i++;
    }
    return (max_val);
}

void	handle_duplicates(int *arr, int *pos, int item)
{
	while (item == arr[*pos])
		(*pos)++;
}

int find_min_index(t_stack *stack)
{
    int min = stack->data[0];
    int idx = 0;
    for (int i = 1; i < stack->size; i++)
    {
        if (stack->data[i] < min)
        {
            min = stack->data[i];
            idx = i;
        }
    }
    return idx;
}

int	ft_min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}