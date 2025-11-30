/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:59:40 by syzygy            #+#    #+#             */
/*   Updated: 2025/11/30 03:22:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OUTPUT_H
#define OUTPUT_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "ft_memory.h"
#include "error.h"

/**
 * OUTPUT_ERR

Constant (octal 01) bit flag used in struct output->flags to record that an output error occurred.
outc (inline)

Two variants depending on USE_GLIBC_STDIO:
If USE_GLIBC_STDIO is defined: outc writes a single character into the FILE* stream using putc(file->stream). Uses stdio buffering and locking.
Otherwise: outc writes a single character into the output buffer: if the buffer is full (file->nextc == file->end) it calls outcslow (slow path that handles flushing/writing), otherwise it stores the char at *nextc and increments nextc (fast buffered path).
doformat(d, f, a)

Macro used only when USE_GLIBC_STDIO is defined. Expands to vfprintf((d)->stream, (f), (a)) — format & write via stdio to the stream in the struct output.
out1c(c)

Convenience macro: call outc(c, out1) — write character c to the primary output (out1 points to the global output).
out2c(c)

out2c(c)

Convenience macro: call outcslow(c, out2) — write character c to the secondary output (err) using the slow path unconditionally.
out1mem(s, l)

Convenience macro: call outmem(s, l, out1) — write l bytes from s to primary output via outmem (buffered/write logic).
out1str(s)

Convenience macro: call outstr(s, out1) — write NUL-terminated string s to primary output.
out2str(s)

Convenience macro: call outstr(s, out2) — write NUL-terminated string s to secondary output (err).
outerr(f)

Macro that expands to (f)->flags — accesses the flags field of an output structure (used to check OUTPUT_ERR, etc).
Short summary: these macros/inline functions are small wrappers to route single chars, strings and memory blocks into either stdio FILE streams (when enabled) or the custom buffered output implementation (fast inline path vs slow/flush path).


 */
#define OUTPUT_ERR 01
#define OUTBUFSIZE 1024
typedef struct s_out
{
    char *nextc;
    char *end;
    char *buf;
    size_t buf_size;
    int fd;
    int flags;
} t_out;

typedef struct s_out_ctx
{
    t_out output;
    t_out prev_err_out;
    t_out errout;
    t_out memout;
    t_out *out1;
    t_out *out2;
} t_out_ctx;

static inline t_out_ctx *get_outs(void)
{
    static t_out_ctx ctx;
    static int inited = 0;

    if (inited)
        return (&ctx);
    ctx.output.buf_size = OUTBUFSIZE;
    ctx.output.fd = STDOUT_FILENO;
    ctx.prev_err_out.buf_size = OUTBUFSIZE;
    ctx.prev_err_out.fd = STDERR_FILENO;
    ctx.errout.buf_size = OUTBUFSIZE;
    ctx.errout.fd = STDERR_FILENO;
    ctx.memout.buf_size = OUTBUFSIZE;
    ctx.memout.fd = STDOUT_FILENO;
    ctx.out1 = &ctx.output;
    ctx.out2 = &ctx.errout;
    inited = 1;
    return (&ctx);
}

void ft_putchar_fd(char c, int fd);
void ft_putendl_fd(char *s, int fd);
void ft_putnbr_base(int nbr, char *radix);
void ft_putnbr_fd(int nb, int fd);
void ft_putstr_fd(char *s, int fd);

/* flush helpers used by error/format modules */
void flushout(t_out *dst);
void flush_all(void);
int ft_write(int fd, const void *p, size_t n);

#endif