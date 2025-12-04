/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 14:58:02 by syzygy            #+#    #+#             */
/*   Updated: 2025/12/04 23:19:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORMAT_H
#define FORMAT_H

#include "ft_stddef.h"
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>
#include "lifoba.h"
#include "ft_memory.h"
#include "output.h"

#ifdef DEBUG
#define TRACE(param) trace param
#define TRACEV(param) tracev param
#else
#define TRACE(param)
#define TRACEV(param)
#endif

/* define the struct itself; output.h provides the typedef 't_state_fd' */
struct s_state_fd
{
	int fd;
	t_state state;
	int exit_status;
};

static inline t_state_fd *get_state_fd(void)
{
	/* initialize all fields explicitly to avoid -Wmissing-field-initializers
	   and ensure the enum field is correctly typed for C++ compilation */
	static t_state_fd a = {.fd = 0, .state = (t_state)0, .exit_status = 0};
	return (&a);
}

/* Variadic / va_list formatting API */
int ft_vdprintf(int fd, const char *format, va_list *ap);
int ft_vlogprintf(t_state_fd *state, const char *format, va_list *ap);
int ft_vsnprintf(char *dst, size_t cap, const char *format, va_list *ap);
int ft_vsprintf(char *dst, const char *format, va_list *ap);
int ft_veprintf(const char *str, va_list *args);
char *ft_vaseprintf(const char *str, va_list *args);

/* user-facing helpers (variadic) */
int ft_dprintf(int fd, const char *format, ...);
int ft_printf(const char *format, ...);
int ft_snprintf(char *dst, size_t cap, const char *format, ...);
int ft_sprintf(char *dst, const char *format, ...);
int ft_aprintf(char **dst, const char *format, ...);

/* utility */
size_t hex_len(size_t n);
size_t uint_len(unsigned int n);

/* WRITER */
#include <stdbool.h>

#define BUF_SIZE 2048

typedef enum e_writer_mode
{
	WRITER_MODE_FD = 0,
	WRITER_MODE_BUF = 1
} t_writer_mode;

typedef struct s_writer
{
	size_t index;
	unsigned char buf[BUF_SIZE];
	int error;
	/* n_written: total characters attempted (what snprintf should return) */
	int n_written;
	/* n_copied: number of bytes actually copied/stored into destination */
	int n_copied;
	int fd;

	/* buffer mode fields */
	t_writer_mode mode;
	char *dst;
	size_t dst_cap; /* bytes available for data (excludes NUL) */
} t_writer;

bool writer_write(t_writer *w, const char *mem, size_t size);
bool writer_write_str(t_writer *w, const char *str);
int writer_flush(t_writer *w);
bool writer_is_full(t_writer *w);
bool writer_putchar(t_writer *w, char c);
void writer_reset(t_writer *w);
bool writer_pad_with(t_writer *w, char c, size_t n);
void writer_write_hex(t_writer *w, size_t n, size_t len, bool upper);
void writer_write_uint(t_writer *w, unsigned int n, size_t len);

/* initialization helpers */
static inline void writer_init_fd(t_writer *w, int fd)
{
	w->mode = WRITER_MODE_FD;
	w->fd = fd;
	w->dst = NULL;
	w->dst_cap = 0;
	writer_reset(w);
}

int writer_terminate_cstr(t_writer *w);

static inline void writer_init_buf(t_writer *w, char *dst, size_t cap_bytes_for_data)
{
	w->mode = WRITER_MODE_BUF;
	w->fd = -1;
	w->dst = dst;
	w->dst_cap = cap_bytes_for_data; /* does NOT include space for NUL */
	writer_reset(w);
}

/* Context for logging: file, function */
typedef struct s_state_fd_ctx
{
	const char *file;
	const char *func;
} t_state_fd_ctx;

/* Parser token metadata */
typedef struct s_token_meta
{
	int flags;
	int width;
	int precision;
	char specifier;
} t_token_meta;

/* Rename parser type to avoid collisions in other modules */
typedef struct s_format_parser
{
	size_t index;
	const char *format;
	va_list *ap;
	t_writer *writer;
	t_token_meta token_meta;
} t_format_parser;

/* Parser API */
int parser_parse_and_write(
	t_format_parser *parser,
	const char *format_str,
	va_list *params,
	t_writer *buf_out);

/* Log print function for user logging (file only) */
int log_print(t_state_fd *state, const char *file, const char *format, ...);
t_state_fd_ctx *set_state_fd_ctx(void);

/* PARSER PRIVATE CONSTANTS */
#define FLAG_MINUS 1
#define FLAG_PLUS 2
#define FLAG_ZERO 4
#define FLAG_HASH 8
#define FLAG_SPACE 16
#define PARAM_ASTERISK -2
#define FLAG_NOT_SET -1

#ifndef DEBUG_MODE
#define DEBUG_MODE false
#endif

typedef enum e_padding_position
{
	PAD_LEFT,
	PAD_RIGHT,
} t_padding_position;

/* likely/unlikely helpers */
#if defined(__clang__) || defined(__GNUC__)
static inline long builtin_expect_long(long x, long expected)
{
	return __builtin_expect(x, expected);
}
#else
static inline long builtin_expect_long(long x, long expected)
{
	(void)expected;
	return x;
}
#endif

static inline int likely(int x) { return (int)builtin_expect_long((long)!!(x), 1); }
static inline int unlikely(int x) { return (int)builtin_expect_long((long)!!(x), 0); }

/* Parser function prototypes (use the format-specific parser type) */
bool parser_process_token(t_format_parser *p);
bool parser_parse_conversion_specifier(t_format_parser *p);
void parser_parse_precision(t_format_parser *p);
void parser_parse_width(t_format_parser *p);
void parser_parse_flags(t_format_parser *p);
void width_padding(t_format_parser *p, size_t len, t_padding_position pos);
void zero_precision_padding(t_format_parser *p, size_t len);
void zero_width_padding(t_format_parser *p, size_t len);
void parser_write_hex(t_format_parser *p, bool upper);
void parser_write_uint(t_format_parser *p);
void parser_write_int(t_format_parser *p);
void parser_write_pointer_address(t_format_parser *p);
void parser_write_string(t_format_parser *p);
void parser_write_char(t_format_parser *p);
void alt_hex(t_format_parser *p, size_t len, bool upper, unsigned int value);

/* Formatting/allocation helpers */
int ft_vasprintf(char **strp, size_t size, const char *fmt, va_list ap);
int ft_asprintf(char **sp, const char *f, ...);
int xasprintf(char **sp, const char *f, ...);
int xvsnprintf(char *outbuf, size_t length, const char *fmt, va_list *ap);

/* compatibility/delegates used throughout codebase */
void do_format(t_out *dest, const char *f, va_list ap);
void outmem(const char *p, size_t len, t_out *dest);
void exverror(int cond, const char *msg, va_list ap);
void sh_error(const char *msg, ...);

int ft_vsnprintf(char *dst, size_t cap, const char *format, va_list *ap);
int ft_vsprintf(char *dst, const char *format, va_list *ap);
int ft_vdprintf(int fd, const char *format, va_list *ap);
int ft_vlogprintf(t_state_fd *state, const char *format, va_list *ap);

size_t hex_len(size_t n);
size_t uint_len(unsigned int n);

#endif /* FORMAT_H */