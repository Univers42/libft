/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 00:49:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"
#include <stdio.h> /* snprintf */

/* Provide fallback if public headers did not define VUNSET */
#ifndef VUNSET
#define VUNSET 0x20
#endif

#ifdef WITH_LINENO

static void write_idx(t_var *v)
{
    t_var_state *st;

    st = get_var_state();
    if (st && v == st->vlineno_ptr && v->text == st->linenovar)
    {
        /* write decimal lineno into the stored LINENO=... buffer suffix */
        /* reserve space for terminating NUL */
        snprintf((char *)st->linenovar + 7, sizeof(st->linenovar) - 7, "%d", st->lineno);
    }
}

#else

static void write_idx(t_var *v)
{
    (void)v;
}

#endif

char *lookup_var(const char *name)
{
    t_var *v;

    v = *find_var(name);
    if (v && !(v->flags & VUNSET))
    {
        write_idx(v);
        return (ft_strchrnul(v->text, '=') + 1);
    }
    return (NULL);
}

intmax_t lookup_var_int(const char *name)
{
    char *s;

    s = lookup_var(name);
    if (!s)
        return (0);
    return (ft_atomax(s, 10));
}

/* simple hash helper */
static unsigned int hash_name(const char *p, unsigned int tabsize)
{
    unsigned int h;

    if (tabsize == 0)
        return (0);
    h = ((unsigned char)*p) << 4;
    while (*p && *p != '=')
    {
        h += (unsigned char)*p;
        p++;
    }
    return (h % tabsize);
}

/* return pointer to bucket for name p */
t_var **var_hash(const char *p)
{
    t_var_state *state;

    state = get_var_state();
    return (&state->vartab[hash_name(p, VTABSIZE)]);
}
