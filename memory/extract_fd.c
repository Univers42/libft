#include <unistd.h>
#include <stdlib.h>
#include "ft_memory.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define EXTRACT_CHUNK 4096

char *extract_fd(int fd)
{
    char buf[EXTRACT_CHUNK];
    char *acc;
    size_t cap;
    size_t len;
    ssize_t r;

    cap = EXTRACT_CHUNK;
    len = 0;
    acc = (char *)xmalloc(cap);
    if (!acc)
        return (NULL);
    while ((r = read(fd, buf, sizeof(buf))) > 0)
    {
        if (len + (size_t)r + 1 > cap)
        {
            cap = (cap + r + EXTRACT_CHUNK);
            acc = (char *)realloc(acc, cap);
            if (!acc)
                return (NULL);
        }
        ft_memcpy(acc + len, buf, (size_t)r);
        len += (size_t)r;
    }
    acc[len] = '\0';
    return (acc);
}

int capture_output(const char *cmd, char *buf, size_t cap)
{
    int pp[2];
    pid_t pid;
    ssize_t n;
    size_t off = 0;

    if (cap == 0 || pipe(pp) == -1)
        return -1;
    pid = fork();
    if (pid == 0)
    {
        dup2(pp[1], STDOUT_FILENO);
        close(pp[0]);
        close(pp[1]);
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127);
    }
    close(pp[1]);
    while ((n = read(pp[0], buf + off, cap - 1 - off)) > 0)
        off += (size_t)n;
    if (off < cap)
        buf[off] = '\0';
    else if (cap > 0)
        buf[cap - 1] = '\0';
    close(pp[0]);
    while (waitpid(pid, NULL, 0) == -1 && errno == EINTR)
        ;
    return 0;
}
