/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:30:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/01 15:36:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <cstring>
#include <string>
#include <vector>

extern "C"
{
#include "../../include/var.h"
#include "../private_var.h"

    /* builtins_var.c */
    int builtin_var_import(const char *assign);
    char **builtin_var_build_env(void);
    int builtin_var_show_all(const char *prefix, int on_flags);
    int builtin_var_set(const char *arg, int flags);
    int builtin_var_unset(const char *name);
    int builtin_var_dump_all(int on_flags); /* new helper */

#ifndef VEXPORT
#define VEXPORT 0x01
#endif
}

/* import current process environ into libvar */
static void import_environ(char **envp)
{
    if (!envp)
        return;
    for (char **p = envp; *p; ++p)
        builtin_var_import(*p);
}

/* split line into tokens separated by spaces (very simple) */
static std::vector<std::string> split_words(const char *line)
{
    std::vector<std::string> out;
    const char *p = line;

    while (*p)
    {
        while (*p == ' ' || *p == '\t' || *p == '\n')
            ++p;
        if (!*p)
            break;
        const char *start = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n')
            ++p;
        out.emplace_back(start, p - start);
    }
    return out;
}

static void print_help()
{
    printf("Commands:\n");
    printf("  :set NAME=VALUE      - set variable\n");
    printf("  :get NAME            - get variable value\n");
    printf("  :unset NAME          - unset variable\n");
    printf("  :export NAME[=VALUE] - set and mark exported\n");
    printf("  :locals              - list all variables\n");
    printf("  :exports             - list exported variables only\n");
    printf("  :env                 - build env from exports and print\n");
    printf("  :int NAME VALUE      - set integer variable\n");
    printf("  :getint NAME         - get integer variable\n");
    printf("  :help                - show this help\n");
    printf("  :quit                - exit repl\n");
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    printf("libvar REPL: isolated env playground\n");
    printf("Type :help for commands, :quit to exit.\n");

    import_environ(envp);

    char buf[4096];
    while (true)
    {
        printf("libvar> ");
        if (!std::fgets(buf, sizeof(buf), stdin))
            break;
        auto words = split_words(buf);
        if (words.empty())
            continue;
        const std::string &cmd = words[0];

        if (cmd == ":quit" || cmd == ":q")
            break;
        else if (cmd == ":help")
            print_help();
        else if (cmd == ":set" && words.size() >= 2)
        {
            if (builtin_var_set(words[1].c_str(), 0) != 0)
                printf("error: set failed\n");
        }
        else if (cmd == ":get" && words.size() >= 2)
        {
            const char *val = lookup_var(words[1].c_str());
            printf("%s -> %s\n", words[1].c_str(), val ? val : "(null)");
        }
        else if (cmd == ":unset" && words.size() >= 2)
        {
            if (builtin_var_unset(words[1].c_str()) != 0)
                printf("error: unset failed\n");
        }
        else if (cmd == ":export" && words.size() >= 2)
        {
            if (builtin_var_set(words[1].c_str(), VEXPORT) != 0)
                printf("error: export failed\n");
        }
        else if (cmd == ":locals")
        {
            /* use dump_all so we print via printf, not out1fmt */
            builtin_var_dump_all(0);
        }
        else if (cmd == ":exports")
        {
            builtin_var_dump_all(VEXPORT);
        }
        else if (cmd == ":env")
        {
            char **env = builtin_var_build_env();
            if (!env)
            {
                printf("(no exported vars)\n");
            }
            else
            {
                for (char **p = env; *p; ++p)
                    printf("%s\n", *p);
                std::free(env); /* list_vars uses heap via xmalloc */
            }
        }
        else if (cmd == ":int" && words.size() >= 3)
        {
            intmax_t v = std::strtoll(words[2].c_str(), nullptr, 10);
            set_varint(words[1].c_str(), v, 0);
        }
        else if (cmd == ":getint" && words.size() >= 2)
        {
            intmax_t v = lookup_varint(words[1].c_str());
            printf("%s -> %" PRIdMAX "\n", words[1].c_str(), v);
        }
        else
        {
            printf("unknown command '%s' (try :help)\n", cmd.c_str());
        }
    }

    libvar_destroy();
    return 0;
}
