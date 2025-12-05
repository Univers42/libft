#include "ft_vector.h"
#include "ft_memory.h"

static bool copy_element(t_vec *v, void *dst, const void *src)
{
    if (!v || !dst || !src)
        return (false);
    if (v->copy_fn)
        return (v->copy_fn(dst, src));
    ft_memcpy(dst, src, v->elem_size);
    return (true);
}

static void free_element(t_vec *v, void *elem)
{
    if (!v || !elem)
        return;
    if (v->free_fn)
        v->free_fn(elem);
    else if (v->type_mask & VEC_TYPE_PTR)
        free(*(void **)elem);
}

static size_t next_capacity(t_vec *v, size_t min_needed)
{
    size_t cap;

    cap = (v->cap ? v->cap : VECTOR_INITIAL_CAPACITY);
    while (cap < min_needed)
        cap *= VECTOR_GROWTH_FACTOR;
    return (cap);
}

bool vec_reserve(t_vec *v, size_t capacity)
{
    void *new_buff;

    if (!v)
        return (false);
    if (capacity <= v->cap)
        return (true);
    new_buff = realloc(v->buff, capacity * v->elem_size);
    if (!new_buff)
        return (false);
    v->buff = new_buff;
    v->cap = capacity;
    return (true);
}

bool vec_insert(t_vec *v, size_t index, const void *elem)
{
    size_t min_cap;
    void *dst;

    if (!v || !elem || index > v->len)
        return (false);
    min_cap = next_capacity(v, v->len + 1);
    if (!vec_reserve(v, min_cap))
        return (false);
    if (index < v->len)
        ft_memmove((char *)v->buff + ((index + 1) * v->elem_size),
                   (char *)v->buff + (index * v->elem_size),
                   (v->len - index) * v->elem_size);
    dst = (char *)v->buff + (index * v->elem_size);
    if (!copy_element(v, dst, elem))
        return (false);
    v->len++;
    return (true);
}

void vec_clear(t_vec *v)
{
    size_t i;

    if (!v || !v->buff)
        return;
    i = 0;
    while (i < v->len)
    {
        free_element(v, (char *)v->buff + (i * v->elem_size));
        i++;
    }
    v->len = 0;
}
