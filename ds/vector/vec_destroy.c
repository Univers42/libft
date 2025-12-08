/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_destroy.c                                      :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

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

void vec_destroy(t_vec *v)
{
    size_t i;

    if (!v)
        return;
    if (v->buff)
    {
        i = 0;
        while (i < v->len)
        {
            free_element(v, (char *)v->buff + (i * v->elem_size));
            i++;
        }
        free(v->buff);
    }
    vec_create(v, sizeof(*v));
}