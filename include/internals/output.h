/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:59:40 by syzygy            #+#    #+#             */
/*   Updated: 2025/12/04 00:17:57 by dlesieur         ###   ########.fr       */
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

 typedef struct s_state_fd t_state_fd;
 
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

typedef struct s_fdio
{
	int     fd;
	int     flags;
	char *io_read_ptr;
	char *io_read_end;
	char *io_read_base;
	char *io_write_base;
	char *io_write_ptr;
	char *io_write_end;
	char *io_buf_base;
	char *io_buf_end;
	char *io_save_base;
	char *io_bak;
	char *save_end;
	struct s_file *chain;
	int flags2;
	unsigned short cur_column;
	signed char vable_offset;
	char short_buf[1];
	void *lock;
} t_fdio;

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

void ft_putchar_fd(const char c, int fd);
void ft_putendl_fd(const char *s, int fd);
void ft_putnbr_base(int nbr, char *radix);
void ft_putnbr_fd(const int nb, int fd);
void ft_putstr_fd(const char *s, int fd);

/* flush helpers used by error/format modules */
void flushout(t_out *dst);
void flush_all(void);
int ft_write(int fd, const void *p, size_t n);
void trputs(const char *s);
void tracev(const char *fmt, va_list va);
void trace(const char *fmt, ...);
void trputc(int c);
void indent(int amount, char *pfx, int fd);
void set_debug(int newval);
void set_trace_fd(int newfd);
int *debug_ptr(void);
int get_trace_fd(void);
int *tracefd_ptr(void);
void    out1fmt(const char *fmt, ...);
void outmem(const char *p, size_t len, t_out *dst);
int log_print(t_state_fd *state, const char *file, const char *format, ...);
int	ft_sprintf(char *dst, const char *format, ...);
int	ft_snprintf(char *dst, size_t cap, const char *format, ...);
int ft_printf(const char *format, ...);
int ft_dprintf(int fd, const char *format, ...);
int	ft_aprintf(char **dst, const char *format, ...);
void flush_all(void);
int fmtstr(char *outbuf, size_t length, const char *fmt, ...);

#endif