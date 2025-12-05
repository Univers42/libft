/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 21:44:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/05 22:06:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
#define READLINE_H

#include "ds.h"
#include "dyn_string.h"
#include <stddef.h>

/* Key codes: printable chars returned as their ASCII code (>0).
   Special keys are negative constants. */
typedef enum e_rl_key
{
    RL_KEY_NONE = 0,
    RL_KEY_LEFT = -1,
    RL_KEY_RIGHT = -2,
    RL_KEY_UP = -3,
    RL_KEY_DOWN = -4,
    RL_KEY_HOME = -5,
    RL_KEY_END = -6,
    RL_KEY_DELETE = -7,
    RL_KEY_BACKSPACE = 127,
    RL_KEY_CTRL_A = -9,
    RL_KEY_CTRL_E = -10,
    RL_KEY_CTRL_L = -11,
} t_rl_key;

/* read one key from fd (blocking). returns either ASCII >=32 or negative special code */
int ft_rl_read_key(int fd);

/* history API */
void rl_history_init(void);
void rl_history_add(const char *line);
char *rl_history_prev(void); /* returns pointer to stored string (do not free) */
char *rl_history_next(void);
void rl_history_reset_index(void);
void rl_history_free(void);

/* history search helpers added for Ctrl+R support */
int rl_history_find_prev(const char *pat, int from_index);
const char *rl_history_get(int idx);
void history(void *hist_arg, int *he, void *unused, int histsize);
t_dyn_str prompt_more_input(t_vec *stack);
t_dyn_str prompt_normal(char *last_cmd_st, size_t st);

#endif
