#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipe.h"

int main(void) {
    t_pipe_fds *pipe_fds;
    char *message = "Hello, Pipeline!";
    char buffer[128];
    ssize_t bytes_read;

    // Create a pipe
    pipe_fds = pipe_create();
    if (!pipe_fds) {
        perror("Failed to create pipe");
        return EXIT_FAILURE;
    }

    // Write to the pipe
    if (write_all(pipe_fds->w, message, strlen(message)) < 0) {
        perror("Failed to write to pipe");
        pipe_close_pair(pipe_fds);
        return EXIT_FAILURE;
    }

    // Close the write end of the pipe
    pipe_close_end(pipe_fds, 1);

    // Read from the pipe
    bytes_read = read_all(pipe_fds->r, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from pipe");
        pipe_close_pair(pipe_fds);
        return EXIT_FAILURE;
    }

    // Null-terminate the buffer and print the message
    buffer[bytes_read] = '\0';
    printf("Read from pipe: %s\n", buffer);

    // Close the read end of the pipe
    pipe_close_pair(pipe_fds);

    return EXIT_SUCCESS;
}