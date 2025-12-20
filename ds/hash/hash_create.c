/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_create.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:15:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 22:20:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hash.h"
#include "ft_memory.h"

int hash_create(t_hash *h, const t_hash_config *config)
{
    if (!h || !config || config->size == 0 || !config->hash_fn || !config->cmp_fn)
        return (-1);
    h->buckets = malloc(sizeof(t_hash_entry *) * config->size);
    if (!h->buckets)
        return (-1);
    ft_memset(h->buckets, 0, sizeof(t_hash_entry *) * config->size);
    h->size = config->size;
    h->count = 0;
    h->hash_fn = config->hash_fn;
    h->cmp_fn = config->cmp_fn;
    h->key_free_fn = config->key_free_fn;
    h->val_free_fn = config->val_free_fn;
    return (0);
}

void hash_destroy(t_hash *h)
{
    size_t i;
    t_hash_entry *entry;
    t_hash_entry *tmp;

    if (!h || !h->buckets)
        return;
    i = 0;
    while (i < h->size)
    {
        entry = h->buckets[i];
        while (entry)
        {
            tmp = entry->next;
            if (h->key_free_fn)
                h->key_free_fn(entry->key);
            if (h->val_free_fn)
                h->val_free_fn(entry->value);
            free(entry);
            entry = tmp;
        }
        i++;
    }
    free(h->buckets);
    ft_memset(h, 0, sizeof(*h));
}

size_t hash_count(const t_hash *h)
{
    if (!h)
        return (0);
    return (h->count);
}

void hash_clear(t_hash *h)
{
    size_t i;
    t_hash_entry *entry;
    t_hash_entry *tmp;

    if (!h || !h->buckets)
        return;
    i = 0;
    while (i < h->size)
    {
        entry = h->buckets[i];
        while (entry)
        {
            tmp = entry->next;
            if (h->key_free_fn)
                h->key_free_fn(entry->key);
            if (h->val_free_fn)
                h->val_free_fn(entry->value);
            free(entry);
            entry = tmp;
        }
        h->buckets[i] = NULL;
        i++;
    }
    h->count = 0;
}
