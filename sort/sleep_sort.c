/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:30:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/06/10 17:24:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_sort.h"
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Structure to pass data to thread
typedef struct s_thread_data
{
	int				value;
	int				*result_array;
	int				*index;
	pthread_mutex_t	*mutex;
}	t_thread_data;

// Thread function that sleeps and then stores the value
static void	*sleep_and_store(void *arg)
{
	t_thread_data	*data;
	int				current_index;

	data = (t_thread_data *)arg;
	usleep(data->value * 1000);
	pthread_mutex_lock(data->mutex);
	current_index = (*data->index)++;
	data->result_array[current_index] = data->value;
	pthread_mutex_unlock(data->mutex);
	return (NULL);
}

// Thread function for simple version (just prints)
static void	*sleep_and_print(void *arg)
{
	t_thread_data	*data;

	data = (t_thread_data *)arg;
	usleep(data->value * 1000);
	pthread_mutex_lock(data->mutex);
	printf("%d ", data->value);
	fflush(stdout);
	pthread_mutex_unlock(data->mutex);
	return (NULL);
}

// Complete sleep sort that returns sorted array
int	*ft_sleep_sort(int *arr, int size)
{
	pthread_t		*threads;
	t_thread_data	*thread_data;
	pthread_mutex_t	mutex;
	int				*result;
	int				index;
	int				i;

	if (!arr || size <= 0)
		return (NULL);
	threads = malloc(size * sizeof(pthread_t));
	thread_data = malloc(size * sizeof(t_thread_data));
	result = malloc(size * sizeof(int));
	if (!threads || !thread_data || !result)
	{
		free(threads);
		free(thread_data);
		free(result);
		return (NULL);
	}
	pthread_mutex_init(&mutex, NULL);
	index = 0;
	i = 0;
	while (i < size)
	{
		thread_data[i].value = arr[i];
		thread_data[i].result_array = result;
		thread_data[i].index = &index;
		thread_data[i].mutex = &mutex;
		pthread_create(&threads[i], NULL, sleep_and_store, &thread_data[i]);
		i++;
	}
	i = 0;
	while (i < size)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_mutex_destroy(&mutex);
	free(threads);
	free(thread_data);
	return (result);
}

// Helper function to print array
static void	print_array(int *arr, int size, char *label)
{
	int	i;

	printf("%s: ", label);
	i = 0;
	while (i < size)
	{
		printf("%d ", arr[i]);
		i++;
	}
	printf("\n");
}

int	main(void)
{
	int	arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
	int	size = 10;
	int	*sorted_result;

	print_array(arr, size, "Original array");
	printf("\n=== Sleep Sort ===\n");
	printf("Sorted values will appear as threads wake up:\n");
	sorted_result = ft_sleep_sort(arr, size);
	if (sorted_result)
	{
		print_array(sorted_result, size, "Sorted result");
		free(sorted_result);
	}
	return (0);
}