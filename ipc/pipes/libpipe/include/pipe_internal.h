#ifndef PIPE_INTERNAL_H
#define PIPE_INTERNAL_H

#include "pipe.h"

typedef struct s_pipe_internal
{
    t_pipe_fds *pipe_fds;
    void *buffer;
    size_t buffer_size;
} t_pipe_internal;

/**
 * Internal function to initialize a pipe with a specified buffer size.
 * Allocates memory for the buffer and sets up the pipe file descriptors.
 * Returns 0 on success, -1 on failure.
 */
int pipe_internal_init(t_pipe_internal *pipe_internal, size_t buffer_size);

/**
 * Internal function to clean up resources associated with a pipe.
 * Closes the pipe file descriptors and frees any allocated memory.
 */
void pipe_internal_cleanup(t_pipe_internal *pipe_internal);

/**
 * Internal function to read data from the pipe into a buffer.
 * Returns the number of bytes read, or -1 on error.
 */
ssize_t pipe_internal_read(t_pipe_internal *pipe_internal);

/**
 * Internal function to write data from a buffer to the pipe.
 * Returns the number of bytes written, or -1 on error.
 */
ssize_t pipe_internal_write(t_pipe_internal *pipe_internal, const void *data, size_t size);

#endif // PIPE_INTERNAL_H