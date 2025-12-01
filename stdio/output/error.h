#ifndef ERROR_H
#define ERROR_H

#include <signal.h>
#include <stdarg.h>
#include <stddef.h>

/* Operation types for errmsg */
#define E_OPEN 01
#define E_CREAT 02
#define E_EXEC 04

/* Exceptions */
#define EXINT 0
#define EXERROR 1
#define EXEND 3
#define EXEXIT 4

/*
 * Error subsystem state (single structure, no file-scope globals here).
 */
typedef struct s_error_state {
    void (*handler)(int);
    int exception;
    int suppressint;
    volatile sig_atomic_t intpending;
    int errlinno;
    int err_fd;
} t_error_state;

/* singleton accessor (must be implemented in a .c file) */
t_error_state *get_error_state(void);

/* error output fd accessors (use fds, not FILE*) */
int get_error_fd(void);
void set_error_fd(int fd);

/* install exception handler callback */
void set_exception_handler(void (*handler)(int));

/* main error API (dash-compatible names) */
void exraise(int e) __attribute__((__noreturn__));
void onint(void) __attribute__((__noreturn__));
void sh_error(const char *fmt, ...) __attribute__((__noreturn__));
void exerror(int cond, const char *fmt, ...) __attribute__((__noreturn__));
const char *errmsg(int e, int action);
void sh_warnx(const char *fmt, ...);

/* Backwards-compatible lightweight names */
void ft_error(const char *fmt, ...);
void ft_warn(const char *fmt, ...);

/* Memory barrier (inline to keep same effect as macro) */
static inline void barrier(void)
{
    __asm__ __volatile__ ("" : : : "memory");
}

/* Interrupt helpers (inline functions replacing macros).
   These call get_error_state() to access the global state. */

static inline void intoff(void)
{
    t_error_state *st = get_error_state();
    st->suppressint++;
    barrier();
}

static inline void inton(void)
{
    t_error_state *st = get_error_state();
    barrier();
    if (--st->suppressint == 0 && st->intpending)
        onint();
}

static inline void error_force_inton(void)
{
    t_error_state *st = get_error_state();
    barrier();
    st->suppressint = 0;
    if (st->intpending)
        onint();
}

static inline int error_saveint(void)
{
    t_error_state *st = get_error_state();
    return st->suppressint;
}

static inline void error_restoreint(int v)
{
    t_error_state *st = get_error_state();
    barrier();
    if ((st->suppressint = v) == 0 && st->intpending)
        onint();
}

static inline void error_clear_pending_int(void)
{
    t_error_state *st = get_error_state();
    st->intpending = 0;
}

static inline int error_int_pending(void)
{
    t_error_state *st = get_error_state();
    return (int)st->intpending;
}

void sh_error(const char *msg, ...);
void exerror(int cond, const char *msg, ...);
void sh_warnx(const char *fmt, ...);
void onint(void);
void exraise(int e);
void ft_error(const char *fmt, ...);
void ft_warn(const char *fmt, ...);
void set_exception_handler(void (*handler)(int));
void set_error_fd(int fd);
int get_error_fd(void);

#endif /* ERROR_H */