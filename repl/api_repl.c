/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   api_repl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 20:52:41 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/20 22:31:03 by marvin           ###   ########.fr       */
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
#include "ft_wctype.h" // for ft_mbrtowc / ft_wcwidth
#include <signal.h> // add if not present
#include "trap.h"

/* forward declarations */
static char *wrap_nonprint(const char *s);
static char *wrap_combining(const char *s); // NEW

/* Parent-level SIGINT handler: keep the REPL process alive when child/readline gets Ctrl+C */
static void repl_parent_sigint(int sig)
{
	(void)sig;
	/* no-op: interrupt is handled by child/readline, parent should continue */
}

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

/* New: return pointer to the active repl state (not a copy). */
t_api_readline *get_repl_state_ptr(void)
{
	return *repl_state_singleton();
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

	/* Ensure we pass a NUL-terminated C string to ft_split to avoid
	   any reads of uninitialized bytes inside the splitter. */
	char *cstr = ft_strdup(file.buff ? file.buff : "");
	/* free dynamic buffer early (we now have a stable NUL-terminated copy) */
	free(file.buff);
	file.buff = NULL;

	temp = ft_split(cstr, ' ');
	free(cstr);
	/* handle unexpected split result */
	if (!temp || !temp[0])
	{
		if (temp)
			free_tab(temp);
		return ft_strdup("0");
	}
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

	/* Prefer user-provided setup_signals() if present */
	if (conf && conf->setup_signals)
		conf->setup_signals();
	else if (!conf || conf->handle_signals)
	{
		/* default: install parent-level SIGINT handler and unwind handlers */
		struct sigaction sa = {0};
		sa.sa_handler = repl_parent_sigint;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sigaction(SIGINT, &sa, NULL);
		set_unwind_sig();
	}

	*meta = (t_api_readline){0};

	/* Initialize readline buffer/state so subsequent reads are safe */
	buff_readline_init(&meta->rl);
	meta->rl.cursor = 0;
	meta->rl.lineno = 1;
	meta->rl.has_finished = false;
	meta->rl.has_line = false;
	meta->rl.should_update_ctx = true;

	meta->pid = getpid_hack();
	meta->context = ft_strdup(argv[0]);
	meta->base_context = ft_strdup(argv[0]);
	set_cmd_status(&meta->last_cmd_status_res, res_status(0), &meta->last_cmd_status_s);
	meta->last_cmd_status_res = res_status(0);
	init_cwd(&meta->cwd);
	meta->env = env_to_vec_env(&meta->cwd, envp);

	/* Initialize history only if enabled in config (defaults = enabled when conf==NULL) */
	if (!conf || conf->enable_history)
		init_history(&meta->hist, &meta->env);
	else
		meta->hist = (t_hist){0}; /* ensure zeroed when history disabled */

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

/* Wrap zero-width (combining) UTF‑8 sequences with \001...\002 so readline
   does not include them in its byte-length calculations (avoids cursor drift).
   If prompt already contains \001, or no combining chars found, returns strdup(s). */
static char *wrap_combining(const char *s)
{
	size_t len = ft_strlen(s);
	size_t i = 0;
	size_t comb_count = 0;
	mbstate_t st;
	wchar_t wc;
	size_t r;

	/* if already contains readline markers, do nothing */
	for (i = 0; i < len; ++i)
		if (s[i] == '\001')
			return ft_strdup(s);

	/* count zero-width codepoints */
	ft_memset(&st, 0, sizeof(st));
	i = 0;
	while (i < len)
	{
		r = ft_mbrtowc(&wc, s + i, MB_CUR_MAX, &st);
		if (r == (size_t)-1 || r == (size_t)-2 || r == 0)
		{
			/* treat as single byte to avoid lockups */
			i++;
			ft_memset(&st, 0, sizeof(st));
			continue;
		}
		if (ft_wcwidth(wc) == 0)
			comb_count++;
		i += r;
	}
	if (comb_count == 0)
		return ft_strdup(s);

	/* allocate new buffer: original + 2 markers per combining sequence + null */
	size_t new_sz = len + comb_count * 2 + 1;
	char *out = malloc(new_sz);
	if (!out)
		return ft_strdup(s);

	ft_memset(&st, 0, sizeof(st));
	i = 0;
	size_t o = 0;
	while (i < len)
	{
		r = ft_mbrtowc(&wc, s + i, MB_CUR_MAX, &st);
		if (r == (size_t)-1 || r == (size_t)-2 || r == 0)
		{
			out[o++] = s[i++];
			ft_memset(&st, 0, sizeof(st));
			continue;
		}
		if (ft_wcwidth(wc) == 0)
		{
			out[o++] = '\001';
			for (size_t k = 0; k < r; ++k)
				out[o++] = s[i + k];
			out[o++] = '\002';
		}
		else
		{
			for (size_t k = 0; k < r; ++k)
				out[o++] = s[i + k];
		}
		i += r;
	}
	out[o] = '\0';
	return out;
}

/* Wrap helper that respects config flag */
static char *wrap_combining_if_enabled(const char *s)
{
	t_repl_config *conf = get_repl_config();
	if (conf && !conf->enable_unicode_prompt)
		return ft_strdup(s);
	/* default: enable when conf == NULL for backwards compat */
	return wrap_combining(s);
}

static void parse_input(t_api_readline *meta)
{
	char *prompt;
	t_parse parser;
	t_deque tt;

	parser = (t_parse){.st = ST_INIT, .stack = {0}};

	/* Ensure locale is set before generating/wrapping prompts so
	   ft_mbrtowc / ft_wcwidth see the correct encoding (fixes unicode cursor drift) */
	ensure_locale();

	// Use user custom prompt if provided
	t_dyn_str prompt_dyn;
	if (meta->prompt_gen)
		prompt_dyn = meta->prompt_gen(&meta->last_cmd_status_res, &meta->last_cmd_status_s);
	else
		prompt_dyn = prompt_normal(&meta->last_cmd_status_res, &meta->last_cmd_status_s);

	/* wrap ANSI sequences so readline sees correct printable length.
	   Then optionally wrap zero-width combining sequences (if enabled). */
	char *wrapped = wrap_nonprint_if_enabled(prompt_dyn.buff);
	free(prompt_dyn.buff);
	prompt_dyn.buff = wrapped;

	/* additional unicode combining handling */
	char *wrapped2 = wrap_combining_if_enabled(prompt_dyn.buff);
	free(prompt_dyn.buff);
	prompt_dyn.buff = wrapped2;

	prompt = prompt_dyn.buff;
	deque_init(&tt, 64, sizeof(t_token), NULL);

	/* build get_more context and call refactored function */
	t_getmore_ctx gm_ctx = {
		.prompt = &prompt,
		.input = &meta->input,
		.last_cmd_status_res = &meta->last_cmd_status_res,
		.last_cmd_status_s = &meta->last_cmd_status_s,
		.input_method = &meta->input_method,
		.context = &meta->context,
		.base_context = &meta->base_context,
		.should_exit = &meta->should_exit,
		.out_tokens = &tt
	};
	get_more_tokens(&meta->rl, &gm_ctx);
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
		/* manage history only when enabled in configuration */
		{
			t_repl_config *rc = get_repl_config();
			if ((rc == NULL || rc->enable_history))
				manage_history_input(&meta.hist, &meta.rl, &meta.input);
		}
		buff_readline_reset(&meta.rl);
		free(meta.input.buff);
		meta.input = (t_dyn_str){0};
	}
	/* restore signal handlers: prefer user restore_signals(), otherwise
	   restore default if we used default handling */
	if (conf && conf->restore_signals)
		conf->restore_signals();
	else if (!conf || conf->handle_signals)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	free_env(&meta.env);
	free_all_state(&meta);
	forward_exit_status(meta.last_cmd_status_res);
}
