/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_queue.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:10:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/04 23:26:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_QUEUE_H
#define FT_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "ft_memory.h"
#include "ft_debug.h"
#include "ft_math.h"

typedef struct s_queue_node
{
	void *data;
	struct s_queue_node *next;
} t_queue_node;

typedef struct s_queue
{
	t_queue_node *front;
	t_queue_node *rear;
	size_t size;
} t_queue;

typedef struct s_deque
{
	void *buf;											   // raw buffer (elem_size)
	size_t cap;											   // capacity in elements
	size_t len;											   // current number of elements
	size_t elem_size;									   // sizeof in byte1
	size_t head;										   // index of first element
	size_t tail;										   // index one past last element
	int (*cmp)(const void *a, const void *b, size_t size); // comp for search/remove (memcmp-like)
	void *ctx;											   // optional user context
} t_deque;

// Core functions
t_queue *ft_queue_create(void);
void ft_queue_destroy(t_queue *queue);
bool ft_queue_enqueue(t_queue *queue, void *data);
void *ft_queue_dequeue(t_queue *queue);

// Access functions
void *ft_queue_front(t_queue *queue);
void *ft_queue_rear(t_queue *queue);

// Utility functions
size_t ft_queue_size(t_queue *queue);
bool ft_queue_is_empty(t_queue *queue);
void ft_queue_clear(t_queue *queue);

// Advanced functions
void ft_queue_print_addresses(t_queue *queue);
void deque_ensure_space(t_deque *ret);
void *deque_idx(t_deque *ret, int idx);
int deque_init(t_deque *ret, size_t initial_cap, size_t elem_size, void *ctx);
void *deque_pop_end(t_deque *ret);
void *deque_pop_start(t_deque *ret);
void deque_push_end(t_deque *ret, void *item);
void deque_push_start(t_deque *ret, void *item);
int deque_clone(t_deque *ret, const t_deque *proto);
void    deque_clear(t_deque *ret);

#endif
