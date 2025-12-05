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

bool vec_remove(t_vec *v, size_t index, void *out_elem)
{
    void *src;
    size_t tail;

    if (!v || index >= v->len)
        return (false);
    src = (char *)v->buff + (index * v->elem_size);
    if (out_elem && !copy_element(v, out_elem, src))
        return (false);
    if (!(v->type_mask & VEC_TYPE_PTR) || !out_elem)
        free_element(v, src);
    tail = v->len - index - 1;
    if (tail)
        ft_memmove(src, (char *)src + v->elem_size, tail * v->elem_size);
    v->len--;
    return (true);
}
