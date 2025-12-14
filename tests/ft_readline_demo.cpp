#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "x_readline.h"

// Helper: append a line to the in-memory history buffer (respect histsize)
static void append_history(const char *line)
{
    if (!line || !*line)
        return;
    t_rl_history *hist = &g_rl.history;
    /* ensure buffer exists */
    if (!hist->history_buff)
        return;
    /* if full, drop oldest */
    if (hist->hist_len >= hist->histsize)
    {
        free(hist->history_buff[0]);
        for (int i = 1; i < hist->hist_len; ++i)
            hist->history_buff[i - 1] = hist->history_buff[i];
        hist->hist_len--;
    }
    hist->history_buff[hist->hist_len++] = strdup(line);
    hist->history_buff[hist->hist_len] = NULL;
    hist->hist_index = hist->hist_len;
}

int main()
{
    const char *histpath = "/tmp/histtest";
    /* set HISTFILE to a stable path so rl_init_history and rl_free_history use it */
    setenv("HISTFILE", histpath, 1);
    /* ensure predictable editing mode */
    setenv("READMODE", "EMACS", 1);

    std::cout << "Initializing readline..." << std::endl;
    init_readline();
    std::cout << "Readline initialized. History capacity: " << g_rl.history.histsize << std::endl;
    std::cout << "History file: " << histpath << std::endl;
    std::cout << "Use arrow Up/Down to navigate history. Type 'exit' or Ctrl-D to quit." << std::endl;

    while (true)
    {
        char *line = ft_readline((char *)"demo> ");
        if (!line)
            break;
        /* treat EOF/empty specially */
        if (line[0] == '\0')
        {
            free(line);
            continue;
        }
        /* quit command */
        if (!strcmp(line, "exit"))
        {
            free(line);
            break;
        }
        std::cout << "You entered: " << line << std::endl;
        append_history(line);
        free(line);
    }

    std::cout << "Persisting history to " << histpath << " ..." << std::endl;
    rl_free_history(&g_rl.history);

    /* optional: dump the saved history file for quick verification */
    std::ifstream histin(histpath);
    if (histin.good())
    {
        std::cout << "Saved history contents:" << std::endl;
        std::string s;
        while (std::getline(histin, s))
            std::cout << "> " << s << std::endl;
        histin.close();
    }
    else
    {
        std::cerr << "Warning: could not open history file: " << histpath << std::endl;
    }

    std::cout << "Exiting." << std::endl;
    return 0;
}
