/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_callbacks.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:07:18 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 22:05:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Weak stubs for shell-specific globals and functions.
   These can be overridden by the shell's own implementations. */

#include <stddef.h> /* added to define NULL for global variables */

/* Command hash table (shell-managed) */
__attribute__((weak)) void *cmdtable = NULL;

/* Builtin location index (shell-managed) */
__attribute__((weak)) int builtinloc = -1;

/* History function (shell-managed, used for HISTSIZE changes) */
__attribute__((weak)) void history(void *hist, void *he, void *unused, int histsize)
{
    (void)hist;
    (void)he;
    (void)unused;
    (void)histsize;
    /* no-op stub */
}

/* Change PATH callback (shell-managed) */
__attribute__((weak)) void change_path(const char *newval)
{
    (void)newval;
    /* no-op stub: shell will override to rebuild cmd hash */
}

/* Helper to clear command table entries (shell-managed) */
__attribute__((weak)) void clear_cmd_entry(void)
{
    /* no-op stub: shell will override to clear hash table */
}
