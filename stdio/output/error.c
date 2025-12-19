/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 17:09:49 by syzygy            #+#    #+#             */
/*   Updated: 2025/12/19 03:08:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"
#include "output.h" /* added to use flush_all() */
#include <errno.h>
#include <stdarg.h>
#include <stdio.h> /* vsnprintf, snprintf */
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* write */

t_error_state *get_error_state(void)
{
    static t_error_state st = {
        .handler = NULL,
        .exception = 0,
        .suppressint = 0,
        .intpending = 0,
        .errlinno = 0,
        .err_fd = 2};
    return (&st);
}

int get_error_fd(void)
{
    return (get_error_state()->err_fd);
}

void set_error_fd(int fd)
{
    if (fd >= 0)
        get_error_state()->err_fd = fd;
}

void set_exception_handler(void (*handler)(int))
{
    get_error_state()->handler = handler;
}

/*
** Write formatted message to configured fd without using FILE*.
*/
static void write_fd_formatted(const char *prefix, const char *msg,
                               va_list ap)
{
    char buf[4096];
    int fd;
    int n;
    int pfxlen;
    int len;

    fd = get_error_fd();
    pfxlen = 0;
    n = 0;
    if (prefix)
        pfxlen = (int)strlen(prefix);
    if (msg)
    {
        n = vsnprintf(buf + pfxlen,
                      (size_t)((sizeof(buf) > (size_t)pfxlen) ? sizeof(buf) - (size_t)pfxlen : 0),
                      msg, ap);
        if (n < 0)
            n = 0;
    }
    if (pfxlen > 0)
        ft_memcpy(buf, prefix, (size_t)pfxlen);
    /* append newline if space allows */
    if ((size_t)(pfxlen + n) < sizeof(buf) - 1)
    {
        buf[pfxlen + n] = '\n';
        n = pfxlen + n + 1;
    }
    else
        n = (int)sizeof(buf);
    len = write(fd, buf, (size_t)n);
    (void)len;
}

/*
** Prefix: "sh: <lineno>: "
*/
static void exvwarning_simple(const char *msg, va_list ap)
{
    char pfx[64];
    int lino;

    lino = get_error_state()->errlinno;
    (void)snprintf(pfx, sizeof(pfx), "sh: %d: ", lino);
    write_fd_formatted(pfx, msg, ap);
}

/*
** Print error message then invoke configured non-returning handler.
*/
static void exverror(int cond, const char *msg, va_list ap)
{
    /* Print a warning like the original implementation */
    exvwarning_simple(msg, ap);

    /* Try to flush internal output buffers instead of using fsync() */
    flush_all();

    /* Invoke the installed exception handler */
    if (get_error_state()->handler == NULL)
        abort();

    get_error_state()->exception = cond;
    /* handler is expected not to return; if it returns, abort to avoid
       continuing in an inconsistent state. */
    get_error_state()->handler(cond);
    abort();
    /* NOTREACHED */
}

/*
** Public wrappers
*/
void sh_error(const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    exverror(EXERROR, msg, ap);
    va_end(ap);
    abort(); /* defensive: ensure noreturn semantics even if exverror returns */
}

void exerror(int cond, const char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    exverror(cond, msg, ap);
    va_end(ap);
    abort(); /* defensive: ensure noreturn semantics even if exverror returns */
}

void sh_warnx(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    exvwarning_simple(fmt, ap);
    va_end(ap);
}

void onint(void)
{
    get_error_state()->intpending = 0;
    exraise(EXINT);
}

void exraise(int e)
{
    if (get_error_state()->handler == NULL)
        abort();
    intoff();
    get_error_state()->exception = e;
    get_error_state()->handler(e);
    abort();
}

/*
** errmsg mirrors original behaviour for ENOENT/ENOTDIR special cases.
*/
const char *errmsg(int e, int action)
{
    if (e != ENOENT && e != ENOTDIR)
        return (strerror(e));
    if (action & E_OPEN)
        return ("No such file");
    if (action & E_CREAT)
        return ("Directory nonexistent");
    return ("not found");
}

/*
** Compatibility helpers that only print a warning (no non-local exit).
*/
void ft_error(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    exvwarning_simple(fmt, ap);
    va_end(ap);
}

void ft_warn(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    exvwarning_simple(fmt, ap);
    va_end(ap);
}
