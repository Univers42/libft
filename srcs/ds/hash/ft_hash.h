/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hash.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 17:15:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 17:05:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HASH_H
#define FT_HASH_H

#include <stdlib.h>
#include <stdbool.h>

typedef void (*t_hash_free_fn)(void *value);
typedef int (*t_hash_cmp_fn)(const void *a, const void *b);
typedef size_t (*t_hash_fn)(const void *key);

/* Hash table entry */
typedef struct s_hash_entry
{
    void *key;
    void *value;
    struct s_hash_entry *next;
} t_hash_entry;

/* Hash table configuration */
typedef struct s_hash_config
{
    size_t size;                /* Number of buckets */
    t_hash_fn hash_fn;          /* Hash function for keys */
    t_hash_cmp_fn cmp_fn;       /* Comparison function for keys */
    t_hash_free_fn key_free_fn; /* Optional: free key */
    t_hash_free_fn val_free_fn; /* Optional: free value */
} t_hash_config;

/* Hash table */
typedef struct s_hash
{
    t_hash_entry **buckets;
    size_t size;
    size_t count;
    t_hash_fn hash_fn;
    t_hash_cmp_fn cmp_fn;
    t_hash_free_fn key_free_fn;
    t_hash_free_fn val_free_fn;
} t_hash;

/* Core operations */
int hash_create(t_hash *h, const t_hash_config *config);
void hash_destroy(t_hash *h);
bool hash_set(t_hash *h, const void *key, void *value);
void *hash_get(t_hash *h, const void *key);
bool hash_remove(t_hash *h, const void *key);
bool hash_contains(t_hash *h, const void *key);

/* Utilities */
size_t hash_count(const t_hash *h);
void hash_clear(t_hash *h);

/* Default hash functions */
size_t hash_string(const void *key);
int cmp_string(const void *a, const void *b);
size_t hash_ptr(const void *key);
int cmp_ptr(const void *a, const void *b);

#endif
