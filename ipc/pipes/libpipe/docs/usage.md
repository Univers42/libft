# Usage of libpipe

## Overview

The `libpipe` library provides a set of utilities for managing inter-process communication (IPC) using pipes. It simplifies the creation, management, and usage of pipes in C programs, allowing for efficient data transfer between processes.

## Installation

To use `libpipe`, include the necessary header files in your project and link against the compiled library. You can build the library using the provided `Makefile` or `CMakeLists.txt`.

## Functions

### Pipe Creation

- **`t_pipe_fds *pipe_create(void);`**
  - Creates a new pipe and returns a pointer to a `t_pipe_fds` structure containing the read and write file descriptors.
  - Returns `NULL` on error.

### Pipe Management

- **`void pipe_close_pair(t_pipe_fds *p);`**
  - Closes both ends of the pipe represented by the `t_pipe_fds` structure.
  
- **`void pipe_close_end(t_pipe_fds *p, int end);`**
  - Closes a specific end of the pipe:
    - `end == 0` for the read end.
    - `end == 1` for the write end.

### Writing Data

- **`ssize_t write_all(int fd, const void *buf, size_t n);`**
  - Writes all bytes from the buffer to the specified file descriptor, handling interruptions.
  - Returns the number of bytes written or `-1` on error.

### Reading Data

- **`ssize_t read_all(int fd, void *buf, size_t n);`**
  - Reads up to `n` bytes from the specified file descriptor, handling interruptions.
  - Returns the number of bytes read or `-1` on error.

- **`char *read_line_arena(int fd);`**
  - Reads a line from the specified file descriptor into a memory arena and returns a dynamically allocated string.
  - Returns `NULL` on error or EOF.

### Setting Flags

- **`int set_nonblock(int fd, int enable);`**
  - Sets or clears the non-blocking mode for the specified file descriptor.

- **`int set_cloexec(int fd, int enable);`**
  - Sets or clears the close-on-exec flag for the specified file descriptor.

## Example Usage

Here is a simple example demonstrating how to use the `libpipe` library to create a pipe and communicate between processes:

```c
#include "pipe.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    t_pipe_fds *pipe_fds = pipe_create();
    if (!pipe_fds) {
        perror("Failed to create pipe");
        return EXIT_FAILURE;
    }

    // Example of writing to the pipe
    const char *message = "Hello, Pipe!";
    write_all(pipe_fds->w, message, ft_strlen(message));

    // Example of reading from the pipe
    char buffer[128];
    ssize_t bytes_read = read_all(pipe_fds->r, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the string
        printf("Read from pipe: %s\n", buffer);
    }

    // Clean up
    pipe_close_pair(pipe_fds);
    free(pipe_fds);
    return EXIT_SUCCESS;
}
```

## Testing

Unit tests are provided for the `write_all` and `read_all` functions to ensure their correctness. You can run the tests using the provided `CMakeLists.txt` in the `tests` directory.

## Conclusion

The `libpipe` library offers a robust and easy-to-use interface for managing pipes in C. By utilizing the provided functions, you can efficiently implement inter-process communication in your applications.