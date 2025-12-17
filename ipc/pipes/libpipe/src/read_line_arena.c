#include "ft_memory.h"
#include "pipe.h"

/*
 * Reads a line from the specified file descriptor into a dynamically allocated buffer.
 * The buffer is managed in an arena, allowing for efficient memory usage.
 * The function returns a pointer to the buffer containing the line, or NULL on error or EOF.
 */
char *read_line_arena(int fd)
{
    char *base;
    char *p;
    int r;

    base = start_stack_str();
    p = base;
    while (1)
    {
        char buf;
        ssize_t read_result = read(fd, &buf, 1);
        if (read_result < 0)
        {
            st_unalloc(base);
            return NULL; // Error occurred
        }
        if (read_result == 0) // EOF
        {
            if (p == base) // No data read
            {
                st_unalloc(base);
                return NULL;
            }
            break; // End of line reached
        }
        *p++ = buf; // Append character to buffer
        if (buf == '\n') // Newline character
            break;
    }
    *p = '\0'; // Null-terminate the string
    grab_stack_str(base); // Commit the string to the arena
    return base; // Return the dynamically allocated string
}
