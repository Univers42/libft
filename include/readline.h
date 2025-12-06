/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 21:44:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/06 18:47:06 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
#define READLINE_H

#include "ds.h"
#include "dyn_string.h"
#include <stddef.h>
#include <stdbool.h>

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

enum e_input_method
{
	INP_READLINE = 0x01,
	INP_FILE = 0x02,
	INP_ARG = 0x04,
	INP_STDIN_NOTTY = 0x08,
};

typedef struct s_rl
{
	bool should_update_ctx;
	int lineno;
	bool has_line;
	size_t cursor;
	bool has_finished;
	t_dyn_str *str;
} t_rl;

typedef struct s_hist
{
	t_vec cmds;
	int append_fd;
	t_dyn_str str;
	bool active;
	size_t iter_idx; /* iteration index for prev/next */
} t_hist;

static inline void rlinit(t_rl *ret)
{
	*ret = (t_rl){};
}

#ifdef __cplusplus
extern "C"
{
#endif

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
	void rlupdate(t_rl *line);
	void rlreset(t_rl *line);
	void update_ctx(t_rl *line, char **ctx, const char *base_ctx);
	int get_more_input_notty(t_rl *rl);
	void bg_readline(int outfd, const char *prompt);
	int attach_input_readline(t_dyn_str *str, int pp[2], int pid);
	int get_more_input_readline(t_dyn_str *str, const char *prompt);
	char *rl_getline(const char *prompt);
	char *extract_fd(int fd);
	char *ft_readline(const char *prompt);

#ifdef __cplusplus
}
#endif

#endif
