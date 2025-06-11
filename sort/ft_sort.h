/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:18:06 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/11 03:08:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SORT_H
#define SORT_H

#include "../memory/ft_memory.h"
#include "../debug/ft_debug.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct s_stack
{
    int *data;
    int size;
}t_stack;

typedef struct s_tagged_element
{
    int value;
    int tag;
}t_tagged_element;

// Structure to pass data to thread
typedef struct s_thread_data
{
	int				value;
	int				*result_array;
	int				*index;
	pthread_mutex_t	*mutex;
}	t_thread_data;

typedef struct s_tree_node
{
    int data;
    struct s_tree_node *left;
    struct s_tree_node *right;
}t_tree_node;

typedef struct s_node
{
    int				value;
    int				index;
    struct s_node	*left;
    struct s_node	*right;
}	t_node;

int     *ft_quick_sort(int *arr, int low, int high);
void    ft_chunk_sort(int *arr, int size);
void	ft_merge_sort(int *arr, int l, int r);
void	merge(int *arr, int l, int m, int r);
int     *ft_bubble_sort(int *arr, int size);
void	cartesian_sort(int *arr, int size);
void    ft_chunk_sort(int *arr, int size);
void	cocktail_shaker_sort(int *arr, int size);
void    comb_sort(int *arr, int size);
void    counting_sort(int *arr, int size);
int     ft_cycle_sort(int *arr, int size);
int     *ft_quick_sort(int *arr, int low, int high);
void	ft_gnome_sort(int *arr, int size);
void    greedy_sort(int *arr, int size);
void    hybrid_sort(int *arr, int size);
void	insertion_sort(int arr[], int left, int right);
void    ksort(int *arr, int n, int k);
void	ft_merge_sort(int *arr, int l, int r);
void	radix_sort(int *arr, int size, int max_bits);
void	ft_selection_sort(int *arr, int n);
int     *ft_sleep_sort(int *arr, int size);
void    strand_sort(int *arr, int size);
void    tag_sort(int *arr, int size);
void	tim_sort(int *arr, int n);
void    tree_sort(int *arr, int size);
int     get_min(int *arr, int size);
int     get_max(int *arr, int size);
void	handle_duplicates(int *arr, int *pos, int item);
int     find_min_index(t_stack *stack);
int     ft_min(int a, int b);
#endif