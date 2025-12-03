/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:06:16 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/03 23:46:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"
#include "ft_stdlib.h"

int var_cmp(const char *p, const char *q)
{
	int c;
	int d;

	/* Defensive: avoid dereferencing NULL pointers. If either side is NULL,
	   treat them as non-equal (unless both NULL, in which case equal). */
	if (p == NULL || q == NULL)
	{
		if (p == q)
			return (0);
		/* ensure a deterministic non-zero result */
		return (p ? 1 : -1);
	}

	while (1)
	{
		c = (unsigned char)*p;
		d = (unsigned char)*q;
		if (c != d)
			break;
		if (!c || c == '=')
			break;
		p++;
		q++;
	}
	if (c == '=')
		c = 0;
	if (d == '=')
		d = 0;
	return (c - d);
}

int var_vpcmp(const void *a, const void *b)
{
	return (var_cmp(*(const char **)a, *(const char **)b));
}

t_var **var_find(struct s_var **vpp, const char *name)
{
	while (*vpp)
	{
		if (var_cmp((*vpp)->text, name) == 0)
			break;
		vpp = &(*vpp)->next;
	}
	return (vpp);
}

t_var **find_var(const char *name)
{
	t_var **vpp;

	vpp = var_hash(name);
	while (*vpp)
	{
		if (var_cmp((*vpp)->text, name) == 0)
			break;
		vpp = &(*vpp)->next;
	}
	return (vpp);
}

intmax_t ft_atomax(const char *s, int base)
{
	char *endptr;

	return ((intmax_t)ft_strto64(s, &endptr, base));
}

void unwind_local_vars(t_localvar_list *stop)
{
	t_var_state *state;

	state = get_var_state();
	while (state->localvar_stack != stop)
		pop_local_vars();
}

char *var_null(const char *s)
{
	const char *p = ft_strchr(s, '=');
	static const char nullstr[] = "";

	if (p)
		return ((char *)p + 1);
	else
		return ((char *)nullstr);
}

/* New: free all variables and local frames managed by libvar.
   Intended to be called once at program shutdown. */
void libvar_destroy(void)
{
	t_var_state *state;
	int i;
	t_var *vp;
	t_var *next;
	t_localvar_list *frame;
	t_localvar_list *next_frame;

	state = get_var_state();
	if (!state)
		return;

	/* Free all vars in hash table */
	i = 0;
	while (i < VTABSIZE)
	{
		vp = state->vartab[i];
		state->vartab[i] = NULL;
		while (vp)
		{
			next = vp->next;
			/* free mutable text: anything not VTEXT_FIXED or VSTACK */
			if ((vp->flags & (VTEXT_FIXED | VSTACK)) == 0 && vp->text)
				xfree((void *)vp->text);
			xfree(vp);
			vp = next;
		}
		i++;
	}

	/* Free all localvar frames */
	frame = state->localvar_stack;
	state->localvar_stack = NULL;
	while (frame)
	{
		next_frame = frame->next;
		//libvar_free_local_frame(frame, state);
		frame = next_frame;
	}

	/* Optionally, clear other dynamic members if you later add any.
	   Static strings in varinit[] and defaults are not heap-owned. */
}