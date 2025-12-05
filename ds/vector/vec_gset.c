#include "ft_vector.h"
#include "ft_memory.h"

static void free_element(t_vec *v, void *elem)
{
    if (!v || !elem)
        return;
    if (v->free_fn)
        v->free_fn(elem);
    else if (v->type_mask & VEC_TYPE_PTR)
        free(*(void **)elem);
}

static bool copy_element(t_vec *v, void *dst, const void *src)
{
    if (!v || !dst || !src)
        return (false);
    if (v->copy_fn)
        return (v->copy_fn(dst, src));
    ft_memcpy(dst, src, v->elem_size);
    return (true);
}

void *vec_get(t_vec *v, size_t index)
{
    if (!v || index >= v->len)
        return (NULL);
    return ((char *)v->buff + (index * v->elem_size));
}

bool vec_set(t_vec *v, size_t index, const void *elem)
{
    void *dst;

    if (!v || index >= v->len || !elem)
        return (false);
    dst = (char *)v->buff + (index * v->elem_size);
    free_element(v, dst);
    return (copy_element(v, dst, elem));
}

size_t vec_size(const t_vec *v)
{
    if (!v)
        return (0);
    return (v->len);
}

size_t vec_capacity(const t_vec *v)
{
    if (!v)
        return (0);
    return (v->cap);
}

bool vec_is_empty(const t_vec *v)
{
    if (!v)
        return (true);
    return (v->len == 0);
}
