#include "pipe.h"

/* read up to n bytes (handles EINTR),
returns bytes read (0 on EOF), -1 on error */
ssize_t read_all(int fd, void *buf, size_t n) {
    char *ptr = (char *)buf;
    size_t left = n;
    ssize_t r;

    while (left) {
        r = read(fd, ptr, left);
        if (r < 0) {
            if (errno == EINTR)
                continue;
            return (-1);
        }
        if (r == 0)
            break;
        left -= (size_t)r;
        ptr += r;
    }
    return ((ssize_t)(n - left));
}