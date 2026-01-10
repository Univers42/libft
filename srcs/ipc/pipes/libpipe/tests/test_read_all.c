#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pipe.h"

#define BUFFER_SIZE 1024

void test_read_all() {
    const char *test_string = "Hello, World!";
    t_pipe_fds *pipe_fds = pipe_create();
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    if (!pipe_fds) {
        perror("Failed to create pipe");
        exit(EXIT_FAILURE);
    }

    // Write to the pipe
    write_all(pipe_fds->w, test_string, ft_strlen(test_string));
    close(pipe_fds->w); // Close the write end after writing

    // Read from the pipe
    bytes_read = read_all(pipe_fds->r, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        perror("Failed to read from pipe");
        pipe_close_pair(pipe_fds);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_read] = '\0'; // Null-terminate the string

    // Check if the read data matches the written data
    if (strcmp(buffer, test_string) == 0) {
        printf("Test passed: Read data matches written data.\n");
    } else {
        printf("Test failed: Read data does not match written data.\n");
    }

    pipe_close_pair(pipe_fds);
}

int main() {
    test_read_all();
    return 0;
}
