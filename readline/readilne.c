/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readilne.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 22:42:44 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 23:24:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include "ds.h"
#include "ft_memory.h"
#include "ft_stdio.h"
#include "readline.h"
#include "trap.h"
#include "ft_string.h"
#include "glob.h"

int builtinloc = -1;
t_tblentry *cmdtable[CMDTABLESIZE] = {0};

int return_last_line(t_rl *rl, t_dyn_str *ret)
{
    int len;

    if (!rl || !rl->str || !ret)
        return (1);
    len = rl->str->len - rl->cursor;
    dyn_str_pushnstr(ret, rl->str->buff + rl->cursor, len);
    rl->cursor = 0;
    rl->str->len = 0;
    rl->has_line = false;
    if (len == 0)
        return (1);
    return (4);
}

int return_new_line(t_rl *rl, t_dyn_str *ret)
{
    char *temp;
    int len;

    if (!rl || !rl->str || !ret)
        return (1);
    rl->lineno++;
    temp = ft_strchr(rl->str->buff + rl->cursor, '\n');
    if (temp == NULL)
        return (return_last_line(rl, ret));
    len = temp - (rl->str->buff + rl->cursor) + 1;
    if (len)
        dyn_str_pushnstr(ret, rl->str->buff + rl->cursor, len);
    rl->cursor += len;
    rl->has_line = rl->cursor != rl->str->len;
    if (len == 0)
        return (1);
    return (4);
}

/**
 * the purpose of status function is to let the user call
 * as needed a function that do something to send back
 * a status to its own strucutre or hting like thatt..
 * it's just an API to let communicate for debugging and tracking
 * !important
 * if nof ufnction is needed pUT Null instead
 */
int buff_readline(t_rl *rl, const char *prompt, int *flag, void (*status)(void *state, void *res))
{
    int code;

    (void)status;
    if (rl->has_finished)
        return (0);
    if (!rl->has_line)
    {
        if ((*flag & INP_ARG) || (*flag & INP_FILE))
            return (rl->has_finished = true, 0);
        if (*flag & INP_STDIN_NOTTY)
            code = get_more_input_notty(rl);
        else
            code = get_more_input_readline(rl->str, prompt);
        if (code == 1)
            return (rl->has_finished = true, 0);
        if (code == 2)
        {
            get_g_sig()->should_unwind = SIGINT;
            return (2);
        }
        if (*flag & INP_READLINE)
            dyn_str_push(rl->str, '\n');
        rl->has_line = true;
    }
    return (return_new_line(rl, rl->str));
}