#include "readline.h"
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* Read one byte, return -1 on error/EOF */
static int read_byte(int fd, char *c)
{
    ssize_t n;

    n = read(fd, c, 1);
    if (n <= 0)
        return -1;
    return 0;
}

/* Decode common ANSI escape sequences */
int rl_read_key(int fd)
{
    char c;
    if (read_byte(fd, &c) == -1)
        return RL_KEY_NONE;
    /* Ctrl combos (ASCII control codes) */
    if (c == 1) /* Ctrl-A */
        return RL_KEY_CTRL_A;
    if (c == 5) /* Ctrl-E */
        return RL_KEY_CTRL_E;
    if (c == 12) /* Ctrl-L */
        return RL_KEY_CTRL_L;
    /* Backspace (some terminals send 127) */
    if ((unsigned char)c == 127)
        return RL_KEY_BACKSPACE;
    /* Escape sequences */
    if (c == '\x1b')
    {
        char seq[3];
        if (read_byte(fd, &seq[0]) == -1)
            return RL_KEY_NONE;
        if (read_byte(fd, &seq[1]) == -1)
            return RL_KEY_NONE;
        if (seq[0] == '[')
        {
            if (seq[1] >= '0' && seq[1] <= '9')
            {
                /* extended: ESC [ 3 ~ etc. */
                if (read_byte(fd, &seq[2]) == -1)
                    return RL_KEY_NONE;
                if (seq[1] == '3' && seq[2] == '~')
                    return RL_KEY_DELETE;
            }
            else
            {
                if (seq[1] == 'A')
                    return RL_KEY_UP;
                if (seq[1] == 'B')
                    return RL_KEY_DOWN;
                if (seq[1] == 'C')
                    return RL_KEY_RIGHT;
                if (seq[1] == 'D')
                    return RL_KEY_LEFT;
                if (seq[1] == 'H')
                    return RL_KEY_HOME;
                if (seq[1] == 'F')
                    return RL_KEY_END;
            }
        }
        /* ESC O H/F also used for Home/End */
        if (seq[0] == 'O')
        {
            if (seq[1] == 'H')
                return RL_KEY_HOME;
            if (seq[1] == 'F')
                return RL_KEY_END;
        }
        return RL_KEY_NONE;
    }
    /* printable ASCII */
    if ((unsigned char)c >= 32 && (unsigned char)c < 127)
        return (int)(unsigned char)c;
    /* otherwise return none */
    return RL_KEY_NONE;
}
