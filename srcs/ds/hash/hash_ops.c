/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:15:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 22:20:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"
#include "ft_memory.h"

void *hash_get(t_hash *h, const void *key)
{
    size_t idx;
    t_hash_entry *entry;

    if (!h || !key)
        return (NULL);
    idx = h->hash_fn(key) % h->size;
    entry = h->buckets[idx];
    while (entry)
    {
        if (h->cmp_fn(entry->key, key) == 0)
            return (entry->value);
        entry = entry->next;
    }
    return (NULL);
}

bool hash_contains(t_hash *h, const void *key)
{
    return (hash_get(h, key) != NULL);
}

bool hash_set(t_hash *h, const void *key, void *value)
{
    size_t idx;
    t_hash_entry *entry;
    t_hash_entry *new_entry;

    if (!h || !key)
        return (false);
    idx = h->hash_fn(key) % h->size;
    entry = h->buckets[idx];
    while (entry)
    {
        if (h->cmp_fn(entry->key, key) == 0)
        {
            if (h->val_free_fn && entry->value)
                h->val_free_fn(entry->value);
            entry->value = value;
            return (true);
        }
        entry = entry->next;
    }
    new_entry = malloc(sizeof(*new_entry));
    if (!new_entry)
        return (false);
    new_entry->key = (void *)key;
    new_entry->value = value;
    new_entry->next = h->buckets[idx];
    h->buckets[idx] = new_entry;
    h->count++;
    return (true);
}

bool hash_remove(t_hash *h, const void *key)
{
    size_t idx;
    t_hash_entry *entry;
    t_hash_entry *prev;

    if (!h || !key)
        return (false);
    idx = h->hash_fn(key) % h->size;
    entry = h->buckets[idx];
    prev = NULL;
    while (entry)
    {
        if (h->cmp_fn(entry->key, key) == 0)
        {
            if (prev)
                prev->next = entry->next;
            else
                h->buckets[idx] = entry->next;
            if (h->key_free_fn)
                h->key_free_fn(entry->key);
            if (h->val_free_fn)
                h->val_free_fn(entry->value);
            free(entry);
            h->count--;
            return (true);
        }
        prev = entry;
        entry = entry->next;
    }
    return (false);
}
