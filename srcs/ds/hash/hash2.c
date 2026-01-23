/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:44:45 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/23 16:48:16 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

/* Initialize hash table with given capacity. */
bool	hash_init(t_hash *h, size_t capacity)
{
	*h = (t_hash){0};
	if (capacity == 0)
		capacity = 64;
	h->elem_size = sizeof(t_hash_entry);
	h->ctx = calloc(capacity, sizeof(t_hash_entry));
	if (h->ctx == NULL)
		return (false);
	h->cap = capacity;
	h->len = 0;
	return (true);
}

/* Simple hash function for strings. */
size_t	hash_func(const char *key, size_t cap)
{
	size_t	hash;

	hash = 5381;
	while (*key)
		hash = ((hash << 5) + hash) + (unsigned char)*key++;
	return (hash % cap);
}

/* Find entry by key. Returns index or -1 if not found. */
int	hash_find_idx(t_hash *h, const char *key)
{
	size_t			idx;
	size_t			i;
	t_hash_entry	*buff;

	buff = (t_hash_entry *)h->ctx;
	if (h->cap == 0 || key == NULL)
		return (-1);
	idx = hash_func(key, h->cap);
	i = -1;
	while (++i < h->cap)
	{
		if (buff[idx].key != NULL
			&& strcmp(buff[idx].key, key) == 0)
			return ((int)idx);
		idx = (idx + 1) % h->cap;
	}
	return (-1);
}

/* Resize hash table and rehash entries. */
bool	hash_resize(t_hash *h)
{
	t_hash			new_h;
	size_t			i;
	t_hash_entry	*buff;

	buff = (t_hash_entry *)h->ctx;
	if (!hash_init(&new_h, h->cap * 2))
		return (false);
	i = -1;
	while (++i < h->cap)
		if (buff[i].key != NULL)
			hash_set(&new_h, buff[i].key, buff[i].value);
	free(h->ctx);
	*h = new_h;
	return (true);
}
