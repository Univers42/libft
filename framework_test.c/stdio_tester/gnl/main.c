#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "get_next_line.h"

int	main(void)
{
    int		fd1, fd2;
    char	*line1;
    char	*line2;
    int		i;

    fd1 = open("testfile1.txt", O_RDONLY);
    fd2 = open("testfile2.txt", O_RDONLY);
    if (fd1 < 0 || fd2 < 0)
    {
        perror("open");
        return (1);
    }
    i = 0;
    while (1)
    {
        line1 = get_next_line(fd1);
        line2 = get_next_line(fd2);
        if (!line1 && !line2)
            break;
        if (line1)
        {
            printf("fd1: %s", line1);
            free(line1);
        }
        if (line2)
        {
            printf("fd2: %s", line2);
            free(line2);
        }
        i++;
    }
    close(fd1);
    close(fd2);
    return (0);
}