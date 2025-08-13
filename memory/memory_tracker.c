/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_tracker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:51:10 by dlesieur          #+#    #+#             */
/*   Updated: 2025/05/28 17:50:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Create: memory_tracker.c
#include "ft_memory.h"

void	init_tracker(t_mem_tracker *tracker)
{
	tracker->ptrs = NULL;
	tracker->count = 0;
	tracker->capacity = 0;
}

void	*safe_malloc(t_mem_tracker *tracker, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	if (tracker->count >= tracker->capacity)
	{
		size_t	new_capacity;
		void	**new_ptrs;

		if (tracker->capacity == 0)
		{
			new_capacity = 10;
		}
		else
		{
			new_capacity = tracker->capacity * 2;
		}
		new_ptrs = realloc(tracker->ptrs, new_capacity * sizeof(void *));
		if (!new_ptrs)
		{
			free(ptr);
			return (NULL);
		}
		tracker->ptrs = new_ptrs;
		tracker->capacity = new_capacity;
	}
	tracker->ptrs[tracker->count++] = ptr;
	return (ptr);
}

void	cleanup_all(t_mem_tracker *tracker)
{
	int	i;

	i = 0;
	while (i < tracker->count)
	{
		if (tracker->ptrs[i])
			free(tracker->ptrs[i]);
		i++;
	}
	if (tracker->ptrs)
		free(tracker->ptrs);
	tracker->count = 0;
	tracker->capacity = 0;
	tracker->ptrs = NULL;
}

//int main(int argc, char **argv)
//{
//    t_mem_tracker tracker;
//    
//}