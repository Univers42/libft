#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pipe.h"

#define BUFFER_SIZE 1024

void test_write_all() {
    int pipe_fds[2];
    char buffer[BUFFER_SIZE];
    ssize_t bytes_written;

    // Create a pipe
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Prepare data to write
    const char *data = "Hello, World!";
    size_t data_length = strlen(data);

    // Write data to the pipe
    bytes_written = write_all(pipe_fds[1], data, data_length);
    if (bytes_written == -1) {
        perror("write_all");
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        exit(EXIT_FAILURE);
    }

    // Close the write end of the pipe
    close(pipe_fds[1]);

    // Read the data back from the pipe
    ssize_t bytes_read = read(pipe_fds[0], buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("read");
        close(pipe_fds[0]);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the read data
    buffer[bytes_read] = '\0';

    // Check if the data read matches the data written
    if (strcmp(buffer, data) == 0) {
        printf("Test passed: Data written and read match.\n");
    } else {
        printf("Test failed: Data mismatch.\n");
    }

    // Close the read end of the pipe
    close(pipe_fds[0]);
}

int main() {
    test_write_all();
    return 0;
}