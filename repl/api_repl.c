/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:52:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 20:54:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"
#include "ipc.h"
#include "var.h"
#include "ipc.h"
#include "lexer.h"
#include <ctype.h> // new include

/* forward declarations */
static char *wrap_nonprint(const char *s); // <-- add forward declaration

/* singleton for config */
static t_repl_config **repl_conf_singleton(void)
{
	static t_repl_config *ptr = NULL;
	return &ptr;
}

void set_repl_config(t_repl_config *conf)
{
	*repl_conf_singleton() = conf;
}

t_repl_config *get_repl_config(void)
{
	return *repl_conf_singleton();
}

static t_api_readline **repl_state_singleton(void)
{
	static t_api_readline *ptr = NULL;
	return &ptr;
}

void set_repl_state(t_api_readline *state)
{
	*repl_state_singleton() = state;
}

void get_repl_state(t_api_readline *out)
{
	t_api_readline *state = *repl_state_singleton();
	if (state && out)
		*out = *state;
}

static void free_all_state(t_api_readline *shell)
{
	free(shell->input.buff);
	shell->input = (t_dyn_str){0};
	free(shell->last_cmd_status_s);
	free(shell->pid);
	free(shell->context);
	free(shell->base_context);
	shell->context = 0;
	shell->base_context = 0;
	free(shell->rl.str.buff);
	free_hist(&shell->hist);
	free(shell->cwd.buff);
}

static char *getpid_hack(void)
{
	int fd;
	t_dyn_str file;
	char *ret;
	char **temp;
	const char *err = "CAnnot get PID.";

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
	{
		warning_error(err);
		return (0);
	}
	dyn_str_init(&file);
	dyn_str_append_fd(&file, fd);
	close(fd);
	temp = ft_split(file.buff, ' ');
	free(file.buff);
	ret = ft_strdup(temp[0]);
	free_tab(temp);
	return (ret);
}

static t_status res_status(int status)
{
	return ((t_status){.status = status, .pid = -1, .c_c = false});
}

static void init_cwd(t_dyn_str *_cwd)
{
	char *cwd;

	dyn_str_init(_cwd);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = getenv("PWD");
	if (cwd)
		dyn_str_pushstr(_cwd, cwd);
	else
		ft_eprintf("Shell-init: getcwd failed: %s\n", strerror(errno));
	free(cwd);
}

static void init_repl(t_api_readline *meta, char **argv, char **envp, t_repl_config *conf)
{
	t_fnctx ctx = {.fn = (void (*)(void *))free_all_state, .arg = meta};

	(void)ctx;
	(void)conf;
	set_unwind_sig();
	*meta = (t_api_readline){0};
	meta->pid = getpid_hack();
	meta->context = ft_strdup(argv[0]);
	meta->base_context = ft_strdup(argv[0]);
	set_cmd_status(&meta->last_cmd_status_res, res_status(0), &meta->last_cmd_status_s);
	meta->last_cmd_status_res = res_status(0);
	init_cwd(&meta->cwd);
	meta->env = env_to_vec_env(&meta->cwd, envp);
	init_history(&meta->hist, &meta->env);
	/* expose config to other modules (prompt) */
	set_repl_config(conf);
	set_repl_state(meta);
	meta->prompt_gen = conf && conf->prompt_gen ? conf->prompt_gen : NULL;
}

/* Wrap ANSI CSI/escape sequences with readline non-printing markers \001...\002
   If the prompt already contains \001 markers, do nothing.
   Wrapping will only be applied if the config requests it (wrap_prompt_nonprint). */
static char *wrap_nonprint_if_enabled(const char *s)
{
	t_repl_config *conf = get_repl_config();
	if (conf && !conf->wrap_prompt_nonprint)
		return ft_strdup(s);
	// fallback: if conf is NULL, keep wrapping enabled (backwards compat)
	return wrap_nonprint(s); /* reuse existing implementation further below */
}

/* Wrap ANSI CSI/escape sequences with readline non-printing markers \001...\002
   If the prompt already contains \001 markers, do nothing. */
static char *wrap_nonprint(const char *s)
{
	size_t i = 0;
	size_t esc_count = 0;
	size_t len = ft_strlen(s);
	int need_wrap = 0;

	/* if user already used readline markers, don't touch */
	for (i = 0; i < len; ++i)
		if (s[i] == '\001')
			return ft_strdup(s);

	/* count ESC sequences to size buffer */
	for (i = 0; i < len; ++i)
	{
		if (s[i] == '\033')
		{
			need_wrap = 1;
			esc_count++;
			/* skip CSI/escape bytes */
			if (i + 1 < len && s[i + 1] == '[')
			{
				size_t j = i + 2;
				while (j < len && (unsigned char)s[j] < 0x40)
					j++;
				if (j < len)
					i = j;
			}
		}
	}
	if (!need_wrap)
		return ft_strdup(s);

	/* allocate: original + 2 markers per escape + null */
	size_t new_sz = len + esc_count * 2 + 1;
	char *out = malloc(new_sz);
	if (!out)
		return ft_strdup(s);
	size_t o = 0;
	for (i = 0; i < len; ++i)
	{
		if (s[i] == '\033')
		{
			/* insert start marker */
			out[o++] = '\001';
			/* copy escape sequence */
			out[o++] = s[i++]; /* ESC */
			if (i < len && s[i] == '[')
			{
				out[o++] = s[i++]; /* '[' */
				while (i < len)
				{
					out[o++] = s[i];
					/* CSI final byte is in range 0x40..0x7E */
					if ((unsigned char)s[i] >= 0x40 && (unsigned char)s[i] <= 0x7E)
					{
						i++;
						break;
					}
					i++;
				}
				i--; /* loop will ++ */
			}
			else
			{
				/* non-CSI escape, copy next byte if any */
				if (i < len)
					out[o++] = s[i];
			}
			/* insert end marker */
			out[o++] = '\002';
		}
		else
		{
			out[o++] = s[i];
		}
	}
	out[o] = '\0';
	return out;
}

static void parse_input(t_api_readline *meta)
{
	char *prompt;
	t_parse parser;
	t_deque tt;

	parser = (t_parse){.st = ST_INIT, .stack = {0}};
	// Use user custom prompt if provided
	t_dyn_str prompt_dyn;
	if (meta->prompt_gen)
		prompt_dyn = meta->prompt_gen(&meta->last_cmd_status_res, &meta->last_cmd_status_s);
	else
		prompt_dyn = prompt_normal(&meta->last_cmd_status_res, &meta->last_cmd_status_s);

	/* wrap ANSI sequences so readline sees correct printable length.
	   We replace prompt_dyn.buff with the wrapped buffer; get_more_tokens
	   will free the buffer (consistent with previous lifecycle). */
	char *wrapped = wrap_nonprint_if_enabled(prompt_dyn.buff);
	free(prompt_dyn.buff);
	prompt_dyn.buff = wrapped;

	prompt = prompt_dyn.buff;
	deque_init(&tt, 64, sizeof(t_token), NULL);
	get_more_tokens(&meta->rl, &prompt, &meta->input, &meta->last_cmd_status_res, &meta->last_cmd_status_s, &meta->input_method, &meta->context, &meta->base_context, &meta->should_exit, &tt);
	if (get_g_sig()->should_unwind)
		set_cmd_status(&meta->last_cmd_status_res, (t_status){.status = CANCELED, .pid = -1, .c_c = true}, &meta->last_cmd_status_s);
	free(parser.stack.buff);
	parser.stack = (t_vec){0};
	if (tt.cap && tt.buf)
		free(tt.buf);
	meta->should_exit |= (((get_g_sig()->should_unwind && meta->input_method != INP_READLINE) || meta->rl.has_finished));
}

void repl(t_repl_config *conf, char **argv, char **envp)
{
	t_api_readline meta;

	init_repl(&meta, argv, envp, conf);
	// run until the REPL requests exit
	while (!meta.should_exit)
	{
		dyn_str_init(&meta.input);
		get_g_sig()->should_unwind = 0;
		parse_input(&meta);
		if (meta.rl.cursor > 1)
			manage_history(&meta.hist, &meta.rl);
		buff_readline_reset(&meta.rl);
		free(meta.input.buff);
		meta.input = (t_dyn_str){0};
	}
	free_env(&meta.env);
	free_all_state(&meta);
	forward_exit_status(meta.last_cmd_status_res);
}
