/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:09:58 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/29 16:20:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"

#ifdef WITH_LINENO

static void	write_idx(t_var *v)
{
    t_var_state *st;

	st = get_var_state();
    if (st && v == st->vlineno_ptr && v->text == st->linenovar)
        fmtstr(st->linenovar + 7, sizeof(st->linenovar) - 7, "%d", st->lineno);
}

#else

static void	write_idx(struct var *v)
{
    (void)v;
}

#endif

char	*lookup_var(const char *name)
{
    t_var	*v;

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
    if (looku_pvar(name))
        return (NULLSTR);
    return (0);
}

t_var	**var_hash(const char *p)
{
	unsigned int	hashval;
	t_var_state		*state;

	state = get_var_state();
	hashval = ((unsigned char)*p) << 4;
	while (*p && *p != '=')
	{
		hashval += (unsigned char)*p;
		p++;
	}
	return (&state->vartab[hashval % VTABSIZE]);
}

unsigned int hash_name(const char *p, unsigned int tabsize)
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

t_var   **var_hash(const char *p)
{
    t_var_state *state;

    state = get_var_state();
    return (&state->vartab[hash_name(p, VTABSIZE)]);
}
