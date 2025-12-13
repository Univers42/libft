#ifndef PIPE_H
#define PIPE_H

#include <fcntl.h>
#include <unistd.h>
#include "ft_memory.h"

typedef struct s_pipe_fds
{
    int r;
    int w;
} t_pipe_fds;

#ifdef __cplusplus
extern "C"
{
#endif

    t_pipe_fds *pipe_create(void);
    void pipe_close_pair(t_pipe_fds *p);
    void pipe_close_end(t_pipe_fds *p, int end);
    ssize_t write_all(int fd, const void *buf, size_t n);
    ssize_t read_all(int fd, void *buf, size_t n);
    char *read_line_arena(int fd);
    int set_nonblock(int fd, int enable);
    int set_cloexec(int fd, int enable);

#ifdef __cplusplus
}
#endif

#endif