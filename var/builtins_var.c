/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:30:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 15:36:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private_var.h"
#include "var.h"
#ifndef __cplusplus
#include "ft_stdio.h"
#endif

/* import a NAME=VALUE C-string into libvar */
int builtin_var_import(const char *assign)
{
    char *s;

    if (!assign)
        return (1);
    s = ft_strdup(assign);
    if (!s)
        return (1);
    if (!set_vareq(s, 0))
    {
        xfree(s);
        return (1);
    }
    return (0);
}

/* export all libvar variables with VEXPORT into an envp-like NULL-terminated array */
char **builtin_var_build_env(void)
{
    char **end;
    char **arr;

    arr = list_vars(VEXPORT, 0, &end);
    /* list_vars already returns a NULL-terminated array (arr[end-arr] == NULL) */
    return (arr);
}

/* Print all variables matching on_flags (0 == no mask, print all) using printf. */
int builtin_var_dump_all(int on_flags)
{
    char **end;
    char **arr;

    arr = list_vars(on_flags, 0, &end);
    if (!arr)
        return (0);
    for (char **p = arr; p && *p; ++p)
        printf("%s\n", *p);
    xfree(arr);
    return ((int)(end - arr));
}

/* convenience: set NAME or NAME=VALUE command-style */
int builtin_var_set(const char *arg, int flags)
{
    const char *eq;

    if (!arg)
        return (1);
    eq = ft_strchr(arg, '=');
    if (eq)
    {
        /* full NAME=VALUE */
        char *s;

        s = ft_strdup(arg);
        if (!s)
            return (1);
        if (!set_vareq(s, flags))
        {
            xfree(s);
            return (1);
        }
        return (0);
    }
    /* just mark variable, or set empty value */
    if (!set_var(arg, "", flags))
        return (1);
    return (0);
}

/* convenience: unset NAME */
int builtin_var_unset(const char *name)
{
    if (!name)
        return (1);
    unset_var(name);
    return (0);
}
