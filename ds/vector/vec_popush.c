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

bool vec_push(t_vec *v, const void *elem)
{
    size_t needed;
    void *dst;

    if (!v || !elem)
        return (false);
    if (v->len == v->cap)
    {
        needed = (v->cap ? v->cap * VECTOR_GROWTH_FACTOR : VECTOR_INITIAL_CAPACITY);
        if (!vec_reserve(v, needed))
            return (false);
    }
    dst = (char *)v->buff + (v->len * v->elem_size);
    if (!copy_element(v, dst, elem))
        return (false);
    v->len++;
    return (true);
}

bool vec_pop(t_vec *v, void *out_elem)
{
    size_t idx;
    void *src;

    if (!v || v->len == 0)
        return (false);
    idx = v->len - 1;
    src = (char *)v->buff + (idx * v->elem_size);
    if (out_elem && !copy_element(v, out_elem, src))
        return (false);
    if (!(v->type_mask & VEC_TYPE_PTR) || !out_elem)
        free_element(v, src);
    v->len--;
    return (true);
}
