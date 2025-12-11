/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 16:30:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/11 11:50:29 by dlesieur         ###   ########.fr       */
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
#include "var.h"

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

/* New: use the REPL API */
extern "C"
{
#include "ft_readline.h"
}

// /* Minimal test entrypoint using the new repl API */
// int main(int argc, char **argv, char **envp)
// {
//     t_repl_config conf;

//     /* minimal zero-init, override a few sensible defaults */
//     memset(&conf, 0, sizeof(conf));
//     conf.input_method = INP_READLINE;
//     conf.handle_signals = true;
//     conf.enable_history = true;
//     /* leave prompt_gen/process_input NULL to use library defaults */

//     /* Run the REPL. Note: repl() forwards exit status and may not return. */
//     repl(&conf, argv, envp);
//     return 0;
// }
