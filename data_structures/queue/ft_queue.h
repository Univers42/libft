/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_queue.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 22:10:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/10/31 00:18:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_QUEUE_H
# define FT_QUEUE_H

# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include "ft_memory.h"
# include "ft_debug.h"
# include "ft_math.h"
typedef struct s_queue_node
{
	void				*data;
	struct s_queue_node	*next;
}						t_queue_node;

typedef struct s_queue
{
	t_queue_node	*front;
	t_queue_node	*rear;
	size_t			size;
}					t_queue;

typedef struct s_deque_char
{
	size_t	cap;
	size_t	len;
	size_t	start;
	size_t	end;
	char	*buff;
}	t_deque_char;
typedef struct s_deque_int
{
	size_t	cap;
	size_t	len;
	size_t	start;
	size_t	end;
	ssize_t	*buff;
}	t_deque_int;

typedef struct s_deque
{
	size_t	cap;
	size_t	len;
	size_t	start;
	size_t	end;
	void	*buff;
}	t_deque;

// Core functions
t_queue			*ft_queue_create(void);
void			ft_queue_destroy(t_queue *queue);
bool			ft_queue_enqueue(t_queue *queue, void *data);
void			*ft_queue_dequeue(t_queue *queue);

// Access functions
void			*ft_queue_front(t_queue *queue);
void			*ft_queue_rear(t_queue *queue);

// Utility functions
size_t			ft_queue_size(t_queue *queue);
bool			ft_queue_is_empty(t_queue *queue);
void			ft_queue_clear(t_queue *queue);

// Advanced functions
void			ft_queue_print_addresses(t_queue *queue);

int		deque_int_init(t_deque_int *ret, int size);
void	deque_int_push_end(t_deque_int *ret, int item);
void	deque_int_push_start(t_deque_int *ret, int item);
int		deque_int_pop_end(t_deque_int *ret);
int		deque_int_pop_start(t_deque_int *ret);
int		deque_int_clone(t_deque_int *ret, const t_deque_int proto);
size_t		deque_int_idx(t_deque_int *ret, size_t idx);

int		deque_char_init(t_deque_char *ret, int size);
void	deque_char_push_end(t_deque_char *ret, char item);
void	deque_char_push_start(t_deque_char *ret, char item);
char	deque_char_pop_end(t_deque_char *ret);
char	deque_char_pop_start(t_deque_char *ret);
int		deque_char_clone(t_deque_char *ret, const t_deque_char proto);
void	deque_char_double_if_needed(t_deque_char *ret);
char	deque_char_idx(t_deque_char *ret, size_t idx);
#endif
