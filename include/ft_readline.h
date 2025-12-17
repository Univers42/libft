/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/05 21:44:00 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/17 03:48:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
#define READLINE_H

#include "ds.h"
#include "dyn_string.h"
#include <stddef.h>
#include <stdbool.h>
#include "internals/parser.h"
#include "ft_time.h"
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
	t_dyn_str str;
} t_rl;

typedef struct s_hist
{
	t_vec cmds;
	int append_fd;
	t_dyn_str str;
	bool active;
	size_t iter_idx; /* iteration index for prev/next */
} t_hist;

typedef struct s_status
{
	int status; //-1 means pending
	int pid;	// -1 means no PID
	bool c_c;
} t_status;

// Callback function types for customization
typedef t_dyn_str (*t_prompt_gen_fn)(t_status *, char **);				 // For generating prompts
typedef bool (*t_process_input_fn)(const char *input, t_status *status); // For processing input (return true to exit)
typedef void (*t_cleanup_fn)(void *user_data);							 // For custom cleanup

typedef struct s_repl_config
{
	// Environment and context
	t_vec *env;			// Environment vector for history and variables
	char *base_context; // Base context string (e.g., "shell")
	char **context;		// Pointer to current context string
	int input_method;	// Input method (e.g., INP_READLINE)
	t_status *status;	// Pointer to current status
	char **status_str;	// Pointer to status string
	// Prompts (can be overridden with functions)
	const char *basic_prompt;	   // Default basic prompt string
	const char *multi_line_prompt; // Default multi-line prompt string
	const char *dquote_prompt;	   // Default double-quote prompt string
	const char *squote_prompt;	   // Default single-quote prompt string
	// Function pointers for customization
	t_prompt_gen_fn prompt_gen;		  // Custom prompt generator (if NULL, use basic_prompt)
	t_process_input_fn process_input; // Custom input processor (if NULL, echo and check for "exit")
	t_cleanup_fn cleanup;			  // Custom cleanup function
	// REPL behavior flags
	bool echo_input;			  // Whether to echo input by default
	bool handle_signals;		  // Whether to handle Ctrl+C/D
	bool enable_history;		  // Whether to enable history management
	bool accumulate_continuation; // Whether to handle backslash continuation
	// User data for callbacks
	void *user_data; // Passed to callbacks
	void *(*fn)(void *data);
} t_repl_config;

typedef struct s_stream_dft_data
{
	t_dyn_str input;
	t_dyn_str cwd;
	t_vec env;
	t_hist hist;
	int input_method;
	char *base_context;
	char *context;
	char *pid;
	char *last_cmd_status_s;
	t_status last_cmd_status_res;
	bool should_exit;
	t_rl rl;
} t_stream_dft_data;

#ifdef __cplusplus
extern "C"
{
#endif

	static inline t_chrono *get_chrono(void)
	{
		static t_chrono g = {0};

		return (&g);
	}
	/* Execution time tracking for prompt */
	static inline void prompt_start_timer(void)
	{
		struct timeval start;

		start = get_chrono()->start;
		gettimeofday(&start, NULL);
	}

	static inline void prompt_stop_timer(void)
	{
		struct timeval end;

		gettimeofday(&end, NULL);
		get_chrono()->last_ms = (end.tv_sec - get_chrono()->start.tv_sec) * 1000 + (end.tv_usec - get_chrono()->start.tv_usec) / 1000;
	}
	void buff_readline_update(t_rl *l);
	void bg_readline(int outfd, char *prompt);
	int attach_input_readline(t_rl *l, int pp[2], int pid);
	int get_more_input_readline(t_rl *l, char *prompt);
	void buff_readline_update(t_rl *l);
	void buff_readline_reset(t_rl *l);
	void buff_readline_init(t_rl *ret);
	void update_context(t_rl *rl, char **context, char **base_context);
	int get_more_input_notty(t_rl *rl);
	void free_tab(char **tab);
	int write_to_file(char *str, int fd);
	void forward_exit_status(t_status res);
	void set_cmd_status(t_status *last_cmd_status_res, t_status res, char **last_cmd_status_s);
	t_dyn_str parse_single_cmd(t_dyn_str hist, size_t *cur);
	t_vec parse_hist_file(t_dyn_str hist);
	char *get_hist_file_path(t_vec *_env);
	void parse_history_file(t_hist *_hist, t_vec *env);
	t_dyn_str encode_cmd_hist(char *cmd);
	bool worthy_of_being_remembered(t_hist *hist, t_rl *rl);
	void manage_history(t_hist *hist, t_rl *rl);
	void init_history(t_hist *hist, t_vec *env);
	void free_hist(t_hist *hist);
	bool is_empty_token_list(t_deque *tokens);
	t_dyn_str prompt_more_input(t_parse *parser);
	t_dyn_str prompt_normal(t_status *last_cmd_status_res, char **last_cmd_status_s);
	int xreadline(t_rl *rl, t_dyn_str *ret, char *prompt, t_status *last_cmd_status_res, char **last_cmd_status_s, int *input_method, char **context, char **base_context);
	void get_more_tokens(t_rl *rl,
						 char **prompt, t_dyn_str *input, t_status *last_cmd_status_res,
						 char **last_cmd_status_s, int *input_method,
						 char **context, char **base_context, bool *should_exit, t_deque *out_tokens);
	int repl_run(t_repl_config *config);
#ifdef __cplusplus
}
#endif

#endif
