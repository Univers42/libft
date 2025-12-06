#include "termcap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readline.h"
#include <unistd.h>
#include <limits.h>
#include "ft_stdio.h"
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include "ft_memory.h"
#include "ft_string.h"
#include "ft_stdlib.h"

/* tputs callback */
static int out_int(int c)
{
    char ch = (char)c;
    write(1, &ch, 1);
    return (c);
}

static void print_banner(void)
{
    const char *banner =
        "\033[38;5;196m"
        "    ██╗  ██╗███████╗██╗     ██╗     ██╗███████╗██╗  ██╗\n"
        "    ██║  ██║██╔════╝██║     ██║     ██║██╔════╝██║  ██║\n"
        "    ███████║█████╗  ██║     ██║     ██║███████╗███████║\n"
        "    ██╔══██║██╔══╝  ██║     ██║     ██║╚════██║██╔══██║\n"
        "    ██║  ██║███████╗███████╗███████╗██║███████║██║  ██║\n"
        "    ╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝╚══════╝╚═╝  ╚═╝\n"
        "\033[0m"
        "\033[38;5;208m    🔥 A blazing fast shell for the brave 🔥\033[0m\n\n";
    ft_putstr_fd(banner, STDOUT_FILENO);
}

static void abbreviate_path(const char *full, char *out, size_t outsz)
{
    const char *home = getenv("HOME");
    size_t hlen;

    if (home && strncmp(full, home, (hlen = strlen(home))) == 0 &&
        (full[hlen] == '/' || full[hlen] == '\0'))
    {
        /* replace home prefix with ~ */
        snprintf(out, outsz, "~%s", full + hlen);
    }
    else
    {
        /* keep last two components if path is long */
        const char *p = strrchr(full, '/');
        if (p && p > full)
        {
            const char *prev = p - 1;
            while (prev > full && *prev != '/')
                prev--;
            if (prev > full)
                snprintf(out, outsz, "...%s", prev);
            else
                snprintf(out, outsz, "%s", full);
        }
        else
            snprintf(out, outsz, "%s", full);
    }
}

static int get_git_status(char *status, size_t sz)
{
    char changes[128] = {0};
    int has_changes = 0;

    if (capture_output("git status --porcelain 2>/dev/null | wc -l", changes, sizeof(changes)) == 0)
    {
        trim_newline(changes);
        int n = atoi(changes);
        if (n > 0)
        {
            snprintf(status, sz, " ±%d", n);
            has_changes = 1;
        }
    }
    return has_changes;
}

static void get_system_info(char *cpu_usage, size_t cpu_sz, char *mem_usage, size_t mem_sz)
{
    char buf[256];

    /* CPU usage - simplified */
    if (capture_output("top -bn1 | grep 'Cpu(s)' | awk '{print $2}' | cut -d'%' -f1 2>/dev/null", buf, sizeof(buf)) == 0)
    {
        trim_newline(buf);
        float cpu = atof(buf);
        snprintf(cpu_usage, cpu_sz, "%.0f%%", cpu);
    }
    else
        snprintf(cpu_usage, cpu_sz, "N/A");

    /* Memory usage */
    if (capture_output("free | grep Mem | awk '{printf \"%.0f\", ($3/$2) * 100.0}' 2>/dev/null", buf, sizeof(buf)) == 0)
    {
        trim_newline(buf);
        snprintf(mem_usage, mem_sz, "%s%%", buf);
    }
    else
        snprintf(mem_usage, mem_sz, "N/A");
}

static void make_prompt(char *buf, size_t bufsz, int last_status)
{
    char cwd[PATH_MAX], abbrev[128], host[64], tbuf[32], branch[128];
    char git_status[64] = {0}, cpu[16], mem[16];
    const char *user = getenv("USER");
    const char *status_glyph = (last_status == 0) ? "✔" : "✗";
    const char *status_color = (last_status == 0) ? "\033[38;5;120m" : "\033[38;5;203m";
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    size_t off = 0;
    int has_git = 0;

    if (!getcwd(cwd, sizeof(cwd)))
        snprintf(cwd, sizeof(cwd), "%s", "?");
    abbreviate_path(cwd, abbrev, sizeof(abbrev));
    if (gethostname(host, sizeof(host)) != 0)
        snprintf(host, sizeof(host), "%s", "local");
    if (!user)
        user = "user";
    if (tm)
        strftime(tbuf, sizeof(tbuf), "%a %H:%M:%S", tm);
    else
        snprintf(tbuf, sizeof(tbuf), "%s", "-- --:--");

    /* Get git branch */
    if (capture_output("git rev-parse --abbrev-ref HEAD 2>/dev/null", branch, sizeof(branch)) == 0 &&
        branch[0] != '\0')
    {
        trim_newline(branch);
        if (strlen(branch) > 20)
        {
            branch[17] = '.';
            branch[18] = '.';
            branch[19] = '.';
            branch[20] = '\0';
        }
        has_git = 1;
        get_git_status(git_status, sizeof(git_status));
    }

    /* Get system info */
    get_system_info(cpu, sizeof(cpu), mem, sizeof(mem));

    /* ═══════════════════════════════════════════════════════════════
     * POWERLINE PROMPT WITH GRADIENT AND RICH METADATA
     * ═══════════════════════════════════════════════════════════════ */

    /* Segment 1: User@Host */
    off += snprintf(buf + off, bufsz - off,
                    "\001\033[0m\033[48;5;55m\033[38;5;141m\002\001\033[0m\002");
    off += snprintf(buf + off, bufsz - off,
                    "\001\033[48;5;55m\002 \001\033[1;38;5;219m\002%s\001\033[0m\033[48;5;55m\033[38;5;183m\002@\001\033[1;38;5;225m\002%s\001\033[0m\002 ",
                    user, host);

    /* Powerline separator: purple arrow on blue background */
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[0m\033[48;5;61m\033[38;5;55m\002\001\033[0m\002");

    /* Segment 2: Directory - Blue gradient (61→67) */
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[48;5;61m\002 \001\033[38;5;117m\002📁\001\033[0m\002 \001\033[48;5;61m\033[1;38;5;229m\002%s\001\033[0m\002 ",
                       abbrev);

    if (has_git)
    {
        /* Powerline separator: blue arrow on teal background */
        off += ft_snprintf(buf + off, bufsz - off,
                           "\001\033[0m\033[48;5;36m\033[38;5;61m\002\001\033[0m\002");
        off += ft_snprintf(buf + off, bufsz - off,
                           "\001\033[48;5;36m\002 \001\033[38;5;15m\002\001\033[0m\002 \001\033[48;5;36m\033[1;38;5;231m\002%s\001\033[0m\002",
                           branch);
        if (git_status[0])
            off += ft_snprintf(buf + off, bufsz - off,
                               "\001\033[48;5;36m\033[38;5;220m\002%s\001\033[0m\002",
                               git_status);
        off += ft_snprintf(buf + off, bufsz - off, "\001\033[48;5;36m\002 \001\033[0m\002");

        /* Powerline separator: teal arrow on gray background */
        off += ft_snprintf(buf + off, bufsz - off,
                           "\001\033[0m\033[48;5;239m\033[38;5;36m\002\001\033[0m\002");
    }
    else
    {
        /* No git - blue arrow on gray background */
        off += ft_snprintf(buf + off, bufsz - off,
                           "\001\033[0m\033[48;5;239m\033[38;5;61m\002\001\033[0m\002");
    }

    /* Segment 3: Time & Date - Dark gray (239→240) */
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[48;5;239m\002 \001\033[38;5;147m\002⏱\001\033[0m\002  \001\033[48;5;239m\033[38;5;252m\002%s\001\033[0m\002 ",
                       tbuf);

    /* Powerline separator: gray arrow on darker gray background */
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[0m\033[48;5;236m\033[38;5;239m\002\001\033[0m\002");
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[48;5;236m\002 \001\033[38;5;210m\002🖥\001\033[0m\002  \001\033[48;5;236m\033[38;5;186m\002%s\001\033[0m\002 ",
                       cpu);
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[48;5;236m\033[38;5;117m\002💾\001\033[0m\002 \001\033[48;5;236m\033[38;5;186m\002%s\001\033[0m\002 ",
                       mem);

    /* Powerline separator: darker gray arrow on darkest background */
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[0m\033[48;5;234m\033[38;5;236m\002\001\033[0m\002");
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[48;5;234m\002 %s%s\001\033[0m\002 ",
                       status_color, status_glyph);

    /* Final powerline edge - close background completely */
    off += ft_snprintf(buf + off, bufsz - off,
                       "\001\033[0m\033[38;5;234m\002\001\033[0m\002");

    /* Multi-level gradient prompt arrow - NO background here */
    off += ft_snprintf(buf + off, bufsz - off,
                       "\n\001\033[38;5;213m\002❯\001\033[38;5;177m\002❯\001\033[38;5;141m\002❯\001\033[0m\002 ");
}

int main(void)
{
    const char *term = getenv("TERM");
    char termbuf[4096];

    if (!term)
        term = "xterm";

    /* try to load termcap once (rl_getline will try too) */
    tgetent(termbuf, term);

    print_banner();
    printf("\033[38;5;248m╭─ Available Commands ─────────────────────────╮\n");
    printf("│ \033[1;38;5;117mclear\033[0m\033[38;5;248m    - Clear the terminal screen          │\n");
    printf("│ \033[1;38;5;117mmove\033[0m\033[38;5;248m R C - Move cursor to row R, column C   │\n");
    printf("│ \033[1;38;5;117mhistory\033[0m\033[38;5;248m  - Display command history             │\n");
    printf("│ \033[1;38;5;117mexit\033[0m\033[38;5;248m     - Exit the shell                     │\n");
    printf("╰──────────────────────────────────────────────╯\033[0m\n\n");

    int last_status = 0;

    /* initialize history explicitly */
    rl_history_init();

    while (1)
    {
        char prompt[1024];
        make_prompt(prompt, sizeof(prompt), last_status);

        /* Set the prompt with readline - this protects it from backspace */
        char *line = ft_readline(prompt);

        if (!line)
            break;

        if (strcmp(line, "exit") == 0)
        {
            free(line);
            last_status = 0;
            break;
        }
        else if (strcmp(line, "clear") == 0)
        {
            char *cl = tgetstr("cl", NULL);
            if (cl)
                tputs(cl, 1, out_int);
            else
                write(1, "\033[H\033[2J", 7);
            last_status = 0;
        }
        else if (strcmp(line, "history") == 0)
        {
            /* demonstrate rl_history_prev/rl_history_next */
            rl_history_reset_index();
            char *entry;
            int i = 0;
            printf("\033[38;5;117m╭─ Command History ───────────────────────╮\033[0m\n");
            while ((entry = rl_history_prev()) != NULL)
            {
                printf("\033[38;5;248m│\033[0m \033[38;5;229m%3d\033[0m \033[38;5;244m│\033[0m %s\n", i++, entry);
            }
            printf("\033[38;5;117m╰─────────────────────────────────────────╯\033[0m\n");
            rl_history_reset_index();
            last_status = 0;
        }
        else if (strncmp(line, "move", 4) == 0)
        {
            int r, c;
            if (sscanf(line + 4, "%d %d", &r, &c) == 2)
            {
                char *cm = tgetstr("cm", NULL);
                if (cm)
                    tputs(tgoto(cm, c, r), 1, out_int);
                else
                {
                    char seq[64];
                    int len = ft_snprintf(seq, sizeof(seq), "\033[%d;%dH", r + 1, c + 1);
                    write(1, seq, len);
                }
                last_status = 0;
            }
            else
            {
                printf("\033[38;5;203m✗ Error:\033[0m Usage: move <row> <col>\n");
                last_status = 1;
            }
        }
        else
        {
            printf("\033[38;5;141m➜\033[0m You said: \033[38;5;229m%s\033[0m\n", line);
            last_status = 0;
        }

        free(line);
    }

    rl_history_free();
    printf("\n\033[38;5;211m♥\033[0m \033[38;5;248mThanks for using Hellish! Stay blazing.\033[0m \033[38;5;211m♥\033[0m\n");
    return (0);
}