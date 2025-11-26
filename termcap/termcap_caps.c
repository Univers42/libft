/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap_caps.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/26 01:23:19 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termcap.h"
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

/* get terminal size: prefer ioctl, fallback to termcap numbers */
int tc_get_size(int *rows, int *cols)
{
    struct winsize ws;
    t_tglob *g;

    if (rows)
        *rows = 0;
    if (cols)
        *cols = 0;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
    {
        if (rows)
            *rows = (int)ws.ws_row;
        if (cols)
            *cols = (int)ws.ws_col;
        return (1);
    }
    g = access_glob();
    if (g->term_entry)
    {
        if (rows)
            *rows = tgetnum("li");
        if (cols)
            *cols = tgetnum("co");
        return (1);
    }
    return (0);
}

/* clear entire screen */
void tc_clear_screen(void)
{
    char *cl;

    cl = tgetstr("cl", NULL);
    if (cl)
    {
        tputs(cl, 1, (int (*)(int))putchar);
        /* try to clear scrollback and ensure home+clear for robustness */
        write(1, "\033[3J\033[H\033[2J", 11);
    }
    else
    {
        /* fallback: home + clear screen + attempt scrollback clear */
        write(1, "\033[H\033[2J\033[3J", 11);
    }
    /* flush to ensure terminal processes sequences immediately */
    fflush(stdout);
}

/* clear to end of line */
void tc_clear_eol(void)
{
    char *ce;

    ce = tgetstr("ce", NULL);
    if (ce)
        tputs(ce, 1, (int (*)(int))putchar);
    else
        write(1, "\033[K", 3);
}

/* move cursor to (row,col) 0-based */
void tc_move_cursor(int row, int col)
{
    char *cm;
    char *seq;

    cm = tgetstr("cm", NULL);
    if (cm)
    {
        seq = tgoto(cm, col, row);
        if (seq)
            tputs(seq, 1, (int (*)(int))putchar);
    }
    else
    {
        char tmp[64];
        snprintf(tmp, sizeof(tmp), "\033[%d;%dH", row + 1, col + 1);
        write(1, tmp, strlen(tmp));
    }
}

/* save / restore cursor */
void tc_save_cursor(void)
{
    char *sc;

    sc = tgetstr("sc", NULL);
    if (sc)
        tputs(sc, 1, (int (*)(int))putchar);
    else
        write(1, "\0337", 2);
}

void tc_restore_cursor(void)
{
    char *rc;

    rc = tgetstr("rc", NULL);
    if (rc)
        tputs(rc, 1, (int (*)(int))putchar);
    else
        write(1, "\0338", 2);
}

/* bold/attribute toggles */
void tc_enter_bold(void)
{
    char *md;

    md = tgetstr("md", NULL);
    if (md)
        tputs(md, 1, (int (*)(int))putchar);
    else
        write(1, "\033[1m", 4);
}

void tc_exit_attr(void)
{
    char *me;

    me = tgetstr("me", NULL);
    if (me)
        tputs(me, 1, (int (*)(int))putchar);
    else
        write(1, "\033[0m", 4);
}
