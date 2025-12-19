/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 21:35:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 04:17:57 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "var.h"

// void    expand_dollar(char *last_cmd_status_s, char *pid, t_vec *env, int *i, t_dyn_str *full_file, char *line)
// {
//     int len;
//     char *env;

//     (*i)++;
//     len = env_len(line + *i);
//     if (len)
//     {
//         env = env_expand_n(last_cmd_status_s, pid, line + *i, len, env);
//         if (env)
//             dyn_str_pushstr(full_file, env);
//     }
//     else
//         dyn_str_push(full_file, line[*i]);
//     *i += len;
// }

// void    expand_bs(int *i, t_dyn_str *full_file, char *line)
// {
//     if (is_escapable(line[*i]))
//         dyn_str_push(full_file, line[*i]);
//     else if (line[*i] != '\n')
//     {
//         dyn_str_push(full_file, '\\');
//         dyn_str_push(full_file, line[*i]);
//     }
//     (*i)++;
// }

// void    expand_line(char *last_cmd_status_s, char *pid, t_vec *env, t_dyn_str *full_file, char *line)
// {
//     int i;
//     bool    bs;

//     i = 0;
//     bs = 0;
//     while (line[i])
//     {
//         if (bs)
//         {
//             expand_bs(&i, full_file, line);
//             bs = false;
//             continue ;
//         }
//         if (line[i] == '$')
//         {
//             expand_dollar(last_cmd_status_s, pid, env, &i, full_file, line);
//             continue ;
//         }
//         else if (line[i] == '\\')
//             bs = true;
//         else
//             dyn_str_push(full_file, line[i]);
//         i++;
//     }
// }
