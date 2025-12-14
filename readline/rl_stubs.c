#include "x_readline.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <term.h>
#include <unistd.h>
#include <errno.h>

/* Provide single-definition of globals referenced by the readline code */
t_readline g_rl;
unsigned char g_rl_flags = RL_INIT_FLAGS;

/* Wrapper to avoid defining the global/variable `reset_shell_mode` symbol
   which may be declared differently by trap.h in other translation units. */
void rl_reset_shell_mode(void)
{
    /* no-op for demo */
    (void)0;
}

/* Minimal rl_get_hist_size implementation:
   Fill history->histsize and history->histfilesize from variables if available,
   falling back to RL_HISTSIZE / RL_HISTFILESIZE. */
void rl_get_hist_size(t_rl_history *history)
{
    char *hs;
    char *hfs;

    if (!history)
        return;
    hs = get_var("HISTSIZE", VARS);
    hfs = get_var("HISTFILESIZE", VARS);
    if (!hs)
        hs = RL_HISTSIZE;
    if (!hfs)
        hfs = RL_HISTFILESIZE;
    history->histsize = atoi(hs);
    history->histfilesize = atoi(hfs);
    /* ensure sane defaults */
    if (history->histsize <= 0)
        history->histsize = atoi(RL_HISTSIZE);
    if (history->histfilesize <= 0)
        history->histfilesize = atoi(RL_HISTFILESIZE);
}

/* Minimal terminfo helpers (safe no-op / empty-string implementations)
   These satisfy linker requirements for the demo binary. Replace with
   proper terminfo/ncurses linking in the final program. */
char *tigetstr(const char *cap)
{
    (void)cap;
    return (char *)"";
}

char *tparm(const char *cap, ...)
{
    (void)cap;
    /* ignore parameters */
    return (char *)"";
}

int setupterm(const char *term, int fd, int *err)
{
    (void)term;
    (void)fd;
    if (err)
        *err = 0;
    /* return OK so caller doesn't treat it as an error */
#ifdef OK
    return OK;
#else
    return 0;
#endif
}

/* Minimal shell-related helpers used by readline sources.
   These are simple defaults that allow the demo to run. */

/* Return environment variable value if present; for READMODE return "EMACS"
   when no variable supplied (protects ft_readline usage that expects non-NULL). */
char *get_var(const char *name, int scope)
{
    (void)scope;
    char *v = getenv(name);
    if (!v && name && strcmp(name, "READMODE") == 0)
        return (char *)"EMACS";
    return v;
}

/* Minimal ft_strsub implementation used by readline autocompletion code */
char *ft_strsub(const char *s, unsigned int start, size_t len)
{
    char *out;

    if (!s)
        return NULL;
    out = (char *)malloc(len + 1);
    if (!out)
        return NULL;
    memcpy(out, s + start, len);
    out[len] = '\0';
    return out;
}

/* Minimal ft_strsplit: split on single char delim, returns malloc'd NULL-terminated array */
char **ft_strsplit(const char *s, char delim)
{
    size_t i, cnt = 0;
    char **res;
    const char *p;
    char *token;
    size_t token_len;

    if (!s)
        return NULL;
    /* count tokens (including empty tokens between consecutive delimiters) */
    p = s;
    if (*p == '\0')
    {
        res = malloc(sizeof(char *) * 1);
        res[0] = NULL;
        return res;
    }
    cnt = 1;
    for (i = 0; p[i]; ++i)
        if (p[i] == delim)
            ++cnt;
    res = malloc(sizeof(char *) * (cnt + 1));
    if (!res)
        return NULL;
    size_t idx = 0;
    const char *start = p;
    for (;;)
    {
        const char *pos = strchr(start, delim);
        if (!pos)
            token_len = strlen(start);
        else
            token_len = (size_t)(pos - start);
        token = malloc(token_len + 1);
        if (!token)
        {
            /* simple cleanup */
            for (size_t k = 0; k < idx; ++k)
                free(res[k]);
            free(res);
            return NULL;
        }
        memcpy(token, start, token_len);
        token[token_len] = '\0';
        res[idx++] = token;
        if (!pos)
            break;
        start = pos + 1;
    }
    res[idx] = NULL;
    return res;
}

/* Return an empty names array (NULL-terminated) for variable-name completion.
   This avoids crashes when code calls ft_dar_sort / rl_autocom_print on the result. */
char **get_vars_names_dar(const char *prefix, int mask)
{
    (void)prefix;
    (void)mask;
    char **res = malloc(sizeof(char *) * 1);
    if (!res)
        return NULL;
    res[0] = NULL;
    return res;
}

/* Minimal set_var implementation (no-op for demo; real impl persists vars).
   Prevents undefined reference when rl_init_history calls it. */
int ft_set_var(const char *name, const char *value, int scope)
{
    (void)name;
    (void)value;
    (void)scope;
    /* no-op for demo — real implementation would store in hash/list */
    return 0;
}

/* Minimal err_exit implementation used across the project.
   Prints a brief error message (optionally with strerror) then exits.
   ctx is unused in this simple implementation but kept for compatibility. */
void err_exit(const char *name, const char *msg, const void *ctx, int err)
{
    (void)ctx;
    if (name)
        fprintf(stderr, "%s: ", name);
    if (msg)
        fprintf(stderr, "%s", msg);
    if (err != NOERROR)
    {
        fprintf(stderr, ": %s", strerror(err));
    }
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}
