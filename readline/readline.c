/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 00:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 23:21:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termcap.h"
#include "readline.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ft_string.h"
#include "ft_stdio.h"

/* If termcap.h on some platforms/builds does not declare these helpers,
   provide local prototypes to avoid implicit-declaration warnings. */
void tc_clear_screen(void);
void tc_move_cursor(int row, int col);

#define RL_BUFSIZE 4096
#define RL_HISTORY_INIT 64

typedef struct s_rl_ctx
{
    char *buf;
    int pos;
    int len;
    int hist_idx;
    char *le;
    char *nd;
    char *ce;
    const char *prompt;
} t_rl_ctx;

/* terminal raw helpers */
static int enable_raw(struct termios *orig)
{
    struct termios raw;

    if (tcgetattr(STDIN_FILENO, orig) == -1)
        return (-1);
    raw = *orig;
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        return (-1);
    return (0);
}

static void disable_raw(const struct termios *orig)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

static int out_putc(int c)
{
    char ch = (char)c;
    ft_putchar_fd(ch, STDOUT_FILENO);
    return (c);
}

/* move helpers */
static void move_left_n(char *le, int n)
{
    if (le)
        while (n-- > 0)
            tputs(le, 1, out_putc);
    else
        while (n-- > 0)
            ft_putchar_fd('\b', STDOUT_FILENO);
}

static void move_right_n(char *nd, int n)
{
    if (nd)
        while (n-- > 0)
            tputs(nd, 1, out_putc);
    else
        while (n-- > 0)
            ft_putstr_fd("\033[C", STDOUT_FILENO);
}

/* initialize rl ctx */
static void rl_init_ctx(t_rl_ctx *r, const char *prompt)
{
    r->buf = (char *)xmalloc(RL_BUFSIZE);
    r->pos = 0;
    r->len = 0;
    r->hist_idx = -1;
    r->prompt = prompt;
    r->le = tgetstr("le", NULL);
    r->nd = tgetstr("nd", NULL);
    r->ce = tgetstr("ce", NULL);
}

/* insert printable at cursor */
static void rl_insert(t_rl_ctx *r, char ch)
{
    size_t n;
    if (r->len + 1 >= RL_BUFSIZE)
        return;
    ft_memmove(r->buf + r->pos + 1, r->buf + r->pos, r->len - r->pos);
    r->buf[r->pos] = ch;
    n = write(1, r->buf + r->pos, r->len - r->pos + 1);
    r->len++;
    r->pos++;
    if (r->len > r->pos)
        move_left_n(r->le, r->len - r->pos);
    (void)n;
}

/* backspace handling */
static void rl_backspace(t_rl_ctx *r)
{
    size_t n;

    if (r->pos == 0)
        return;
    move_left_n(r->le, 1);
    ft_memmove(r->buf + r->pos - 1, r->buf + r->pos, r->len - r->pos);
    r->len--;
    r->pos--;
    n = write(1, r->buf + r->pos, r->len - r->pos);
    ft_putchar_fd(' ', STDOUT_FILENO);
    move_left_n(r->le, r->len - r->pos + 1);
    (void)n;
}

/* replace current buffer with given history entry */
static void rl_replace_with_history(t_rl_ctx *r, const char *entry)
{
    if (!entry)
        return;
    ft_putchar_fd('\r', STDOUT_FILENO);
    if (r->prompt)
        ft_putstr_fd((const char *)r->prompt, STDOUT_FILENO);
    if (r->ce)
        tputs(r->ce, 1, out_putc);
    else
        ft_putstr_fd("\033[K", STDOUT_FILENO);
    ft_strncpy(r->buf, entry, RL_BUFSIZE - 1);
    r->len = (int)ft_strlen(r->buf);
    r->pos = r->len;
    if (r->len > 0)
        ft_putstr_fd(r->buf, STDOUT_FILENO);
}

/* display reverse-i-search prompt with pattern and match (match may be NULL) */
static void rl_display_search(const char *pat, const char *match, const char *prompt)
{
    /* prompt like: (reverse-i-search)`pat': match */
    ft_putchar_fd('\r', STDOUT_FILENO);
    if (prompt)
        ft_putstr_fd(prompt, STDOUT_FILENO); /* keep underlying prompt shown if desired */
    /* print small search prompt */
    ft_putstr_fd("(reverse-i-search)`", STDOUT_FILENO);
    if (pat && *pat)
        ft_putstr_fd(pat, STDOUT_FILENO);
    ft_putstr_fd("': ", STDOUT_FILENO);
    if (match)
        ft_putstr_fd(match, STDOUT_FILENO);
    else
        ft_putstr_fd("", STDOUT_FILENO);
}

/* reverse incremental search; returns 1 if a match was accepted into r->buf, 0 on cancel */
static int rl_reverse_search(t_rl_ctx *r)
{
    char pat[256];
    int plen = 0;
    int match_idx = -1;
    const char *match;
    char c;
    int n;
    char orig_buf[RL_BUFSIZE];

    /* save original buffer */
    if (r->len > 0)
        ft_memcpy(orig_buf, r->buf, r->len);
    orig_buf[r->len] = '\0';

    pat[0] = '\0';
    /* initial search (empty -> no match) */
    match = NULL;
    rl_display_search(pat, match, r->prompt);

    while (1)
    {
        n = read(STDIN_FILENO, &c, 1);
        if (n <= 0)
            break;
        /* Enter: accept current match (if any) */
        if (c == '\r' || c == '\n')
        {
            if (match)
            {
                /* replace buffer with match and return accepted */
                rl_replace_with_history(r, match);
                return 1;
            }
            /* nothing to accept: just exit search keeping original */
            rl_replace_with_history(r, orig_buf);
            return 0;
        }
        /* Ctrl-G or ESC => cancel search */
        if (c == 0x07 || c == 0x1b)
        {
            /* restore original buffer */
            rl_replace_with_history(r, orig_buf);
            return 0;
        }
        /* Backspace: remove last char from pattern */
        if (c == 127 || c == 8)
        {
            if (plen > 0)
                pat[--plen] = '\0';
            /* reset search to last entry */
            match_idx = rl_history_find_prev(pat, -1);
            match = match_idx >= 0 ? rl_history_get(match_idx) : NULL;
            rl_display_search(pat, match, r->prompt);
            continue;
        }
        /* Ctrl-R repeats search (find earlier match) */
        if (c == 0x12) /* Ctrl-R */
        {
            if (plen == 0)
                match_idx = rl_history_find_prev(pat, match_idx);
            else
                match_idx = rl_history_find_prev(pat, match_idx);
            match = match_idx >= 0 ? rl_history_get(match_idx) : NULL;
            rl_display_search(pat, match, r->prompt);
            continue;
        }
        /* printable char: add to pattern */
        if ((unsigned char)c >= 32 && (unsigned char)c < 127 && plen + 1 < (int)sizeof(pat))
        {
            pat[plen++] = c;
            pat[plen] = '\0';
            /* search from end for first match */
            match_idx = rl_history_find_prev(pat, -1);
            match = match_idx >= 0 ? rl_history_get(match_idx) : NULL;
            rl_display_search(pat, match, r->prompt);
            continue;
        }
        /* ignore others */
    }
    /* on EOF or error, restore original */
    rl_replace_with_history(r, orig_buf);
    return 0;
}

/* minimal readline with small helpers */
char *rl_getline(const char *prompt)
{
    struct termios orig;
    t_rl_ctx r;
    char c;
    int n;
    char *res;
    char *entry;

    rl_init_ctx(&r, prompt);
    rl_history_init();        /* initialize history storage */
    rl_history_reset_index(); /* start with index after last entry */

    if (enable_raw(&orig) == -1)
        return (NULL);
    if (r.prompt)
        ft_putstr_fd(r.prompt, STDOUT_FILENO);
    while (1)
    {
        n = read(STDIN_FILENO, &c, 1);
        if (n <= 0)
            break;
        if (c == '\r' || c == '\n')
        {
            ft_putstr_fd("\r\n", STDOUT_FILENO);
            break;
        }
        /* Ctrl-L: clear screen and redraw prompt + line */
        if (c == '\x0c')
        {
            /* Clear full screen + scrollback and position at home */
            tc_clear_screen();
            tc_move_cursor(0, 0);

            /* Ensure current line is cleared (erase line) and cursor at col 0 */
            /* Use termcap ce if available, otherwise ANSI ESC[2K + CR */
            if (r.ce)
                tputs(r.ce, 1, out_putc);
            else
                ft_putstr_fd("\033[2K\r", STDOUT_FILENO); /* erase entire line and return carriage */

            /* Reset history navigation state so Up/Down behave from end */
            rl_history_reset_index();

            /* Redraw prompt and current buffer exactly once */
            if (r.prompt)
                ft_putstr_fd(r.prompt, STDOUT_FILENO);
            if (r.len > 0)
                ft_putstr_fd(r.buf, STDOUT_FILENO);

            /* make sure the output is flushed */
            fflush(stdout);
            continue;
        }
        if (c == 127 || c == 8)
        {
            rl_backspace(&r);
            continue;
        }
        if (c == '\x01') /* Ctrl-A: move to start */
        {
            move_left_n(r.le, r.pos);
            r.pos = 0;
            continue;
        }
        if (c == '\x05') /* Ctrl-E: move to end */
        {
            move_right_n(r.nd, r.len - r.pos);
            r.pos = r.len;
            continue;
        }
        if (c == '\x12') /* Ctrl-R: reverse search */
        {
            rl_reverse_search(&r);
            continue;
        }
        if (c == '\x1b') /* escape sequences */
        {
            char seq[3];

            /* read next two bytes */
            if (read(STDIN_FILENO, &seq[0], 1) <= 0)
                continue;
            if (read(STDIN_FILENO, &seq[1], 1) <= 0)
                continue;
            if (seq[0] == '[')
            {
                if (seq[1] == 'D') /* left */
                {
                    if (r.pos > 0)
                    {
                        move_left_n(r.le, 1);
                        r.pos--;
                    }
                    continue;
                }
                if (seq[1] == 'C') /* right */
                {
                    if (r.pos < r.len)
                    {
                        move_right_n(r.nd, 1);
                        r.pos++;
                    }
                    continue;
                }
                if (seq[1] == 'A') /* up: previous history */
                {
                    entry = rl_history_prev();
                    if (entry)
                        rl_replace_with_history(&r, entry);
                    continue;
                }
                if (seq[1] == 'B') /* down: next history or clear */
                {
                    entry = rl_history_next();
                    if (entry && *entry)
                        rl_replace_with_history(&r, entry);
                    else
                    {
                        /* clear line */
                        ft_putchar_fd('\r', STDOUT_FILENO);
                        if (r.prompt)
                            ft_putstr_fd(r.prompt, STDOUT_FILENO);
                        if (r.ce)
                            tputs(r.ce, 1, out_putc);
                        r.len = r.pos = 0;
                        r.buf[0] = '\0';
                        rl_history_reset_index();
                    }
                    continue;
                }
            }
            continue;
        }
        if ((unsigned char)c >= 32 && (unsigned char)c < 127)
        {
            rl_insert(&r, c);
            continue;
        }
    }
    if (r.len > 0)
    {
        r.buf[r.len] = '\0';
        res = xmalloc(r.len + 1);
        if (res)
        {
            ft_memcpy(res, r.buf, r.len);
            res[r.len] = '\0';
        }
        rl_history_add(res);
    }
    else
        res = ft_strdup("");
    disable_raw(&orig);
    xfree(r.buf);
    return (res);
}
