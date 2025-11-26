#ifndef ERROR_H
#define ERROR_H

#include <signal.h>
#include <stdarg.h>

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
** Error subsystem state (no file-scope globals).
** err_fd is the file descriptor used for error output.
*/
typedef struct s_error_state
{
    void (*handler)(int);
    int exception;
    int suppressint;
    volatile sig_atomic_t intpending;
    int errlinno;
    int err_fd;
} t_error_state;

/* singleton accessor */
t_error_state *get_error_state(void);

/* error output fd accessors (use fds, not FILE*) */
int get_error_fd(void);
void set_error_fd(int fd);

/* install exception handler callback */
void set_exception_handler(void (*handler)(int));

/* interrupt helpers (replace dash macros) */
void error_int_off(void);
void error_int_on(void);
void error_force_inton(void);
int error_saveint(void);
void error_restoreint(int v);
void error_clear_pending_int(void);
int error_int_pending(void);

/* main error API (dash-compatible names) */
void exraise(int e) __attribute__((__noreturn__));
void onint(void) __attribute__((__noreturn__));
void sh_error(const char *fmt, ...)
    __attribute__((__noreturn__));
void exerror(int cond, const char *fmt, ...)
    __attribute__((__noreturn__));
const char *errmsg(int e, int action);
void sh_warnx(const char *fmt, ...);

/* Backwards compatibility lightweight names */
void ft_error(const char *fmt, ...);
void ft_warn(const char *fmt, ...);

#endif