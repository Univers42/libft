/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_scanf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syzygy <syzygy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 01:00:39 by syzygy            #+#    #+#             */
/*   Updated: 2025/08/15 01:13:21 by syzygy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ...existing code...
#include "ft_scanf.h"
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

static int read_int(int *out)
{
    int c;
    long val = 0;
    int sign = 1;
    int digits = 0;

    /* skip leading whitespace */
    do {
        c = getchar();
        if (c == EOF)
            return -1;
    } while (isspace(c));

    if (c == '+' || c == '-')
    {
        if (c == '-')
            sign = -1;
        c = getchar();
        if (c == EOF)
            return -1;
    }

    while (c != EOF && isdigit(c))
    {
        val = val * 10 + (c - '0');
        digits++;
        c = getchar();
    }

    if (c != EOF)
        ungetc(c, stdin);

    if (digits == 0)
        return 0; /* mismatch */

    val *= sign;
    if (val > INT_MAX) val = INT_MAX;
    if (val < INT_MIN) val = INT_MIN;
    *out = (int)val;
    return 1;
}

static int read_string(char *buf)
{
    int c;
    size_t i = 0;

    /* skip leading whitespace */
    do {
        c = getchar();
        if (c == EOF)
            return -1;
    } while (isspace(c));

    /* read until next whitespace or EOF */
    while (c != EOF && !isspace(c))
    {
        buf[i++] = (char)c;
        c = getchar();
    }

    if (c != EOF)
        ungetc(c, stdin);

    buf[i] = '\0';
    return 1;
}

static int read_char(char *out)
{
    int c = getchar();
    if (c == EOF)
        return -1;
    *out = (char)c;
    return 1;
}

int vscanf(const char *fmt, va_list ap)
{
    const char *p = fmt;
    int assigned = 0;

    while (*p)
    {
        if (isspace((unsigned char)*p))
        {
            /* skip any whitespace in format: consume any input whitespace */
            int c;
            do {
                c = getchar();
                if (c == EOF)
                    return (assigned == 0) ? EOF : assigned;
            } while (isspace(c));
            if (c != EOF)
                ungetc(c, stdin);
            p++;
            continue;
        }

        if (*p != '%')
        {
            /* match literal character */
            int c = getchar();
            if (c == EOF)
                return (assigned == 0) ? EOF : assigned;
            if (c != (unsigned char)*p)
            {
                ungetc(c, stdin);
                return (assigned == 0) ? EOF : assigned;
            }
            p++;
            continue;
        }

        /* at a '%' */
        p++;
        if (*p == '%')
        {
            /* literal '%' */
            int c = getchar();
            if (c == EOF)
                return (assigned == 0) ? EOF : assigned;
            if (c != '%')
            {
                ungetc(c, stdin);
                return (assigned == 0) ? EOF : assigned;
            }
            p++;
            continue;
        }

        /* handle a few common specifiers: d, s, c */
        if (*p == 'd')
        {
            int *ip = va_arg(ap, int *);
            int r = read_int(ip);
            if (r == -1)
                return (assigned == 0) ? EOF : assigned;
            if (r == 0)
                return assigned;
            assigned++;
            p++;
            continue;
        }
        else if (*p == 's')
        {
            char *s = va_arg(ap, char *);
            int r = read_string(s);
            if (r == -1)
                return (assigned == 0) ? EOF : assigned;
            if (r == 0)
                return assigned;
            assigned++;
            p++;
            continue;
        }
        else if (*p == 'c')
        {
            char *cptr = va_arg(ap, char *);
            int r = read_char(cptr);
            if (r == -1)
                return (assigned == 0) ? EOF : assigned;
            assigned++;
            p++;
            continue;
        }
        else
        {
            /* unsupported specifier: treat as mismatch */
            return (assigned == 0) ? EOF : assigned;
        }
    }

    return assigned;
}

int ft_scanf(const char *fmt, ...)
{
    va_list ap;
    int return_value;

    va_start(ap, fmt);
    return_value = vscanf(fmt, ap);
    va_end(ap);
    return return_value;
}
