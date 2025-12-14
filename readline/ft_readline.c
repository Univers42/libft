/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:16:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:16:27 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

static void win_handler(int sign)
{
	short pos;

	if (sign == SIGWINCH)
	{
		g_rl_flags = RL_INIT_FLAGS;
		pos = g_rl.cord.pos;
		rl_get_cur_cord(&g_rl.cord);
		rl_go_to_cord(0, g_rl.cord.y_cur);
		ft_putstr(tigetstr("ed"));
		rl_write_prompt(g_rl.prompt, g_rl.history);
		rl_start_cord_data(&g_rl.cord);
		rl_print(g_rl.line.buffer + g_rl.cord.pos, &g_rl.cord);
		rl_go_left(g_rl.cord.pos - pos, &g_rl.cord);
		rl_set_non_canon_mode(&g_rl.non_canon_mode);
	}
}

static void rl_find_template(t_readline *rl, char *c)
{
	void (*handler)(t_readline *rl);

	if (rl->mode == VI)
	{
		if (ft_isprint(*c) && !(g_rl_flags & RL_VI_COMMAND_MODE))
			rl_print_symb(c, rl);
		else if ((handler = get_hash_data(rl->vi_temp, c, VI_HASH_SIZE)))
			handler(rl);
	}
	else if (rl->mode == EMACS)
	{
		if (ft_isprint(*c))
			rl_print_symb(c, rl);
		else if ((handler = get_hash_data(rl->emacs_temp, c, EMACS_HASH_SIZE)))
			handler(rl);
	}
}

static char *rl_reading(t_readline *rl)
{
	char c[RL_MAX_BUFF + 1];

	rl_start_cord_data(&rl->cord);
	if (rl->mode == VI)
		g_rl_flags |= RL_VI_INPUT_MODE;
	while (READING)
	{
		signal(SIGWINCH, win_handler);
		rl_read_handler(c, STDIN_FILENO);
		rl_find_template(rl, c);
		if (g_rl_flags & RL_BREAK_FLAG)
			break;
	}
	return (rl->line.buffer);
}

char *ft_readline(char *prompt)
{
	char *buff;
	char *mode;

	mode = get_var("READMODE", VARS);
	if (!ft_strcmp(mode, "VI"))
		g_rl.mode = VI;
	else
		g_rl.mode = EMACS;
	g_rl.prompt = prompt;
	rl_write_prompt(g_rl.prompt, g_rl.history);
	rl_clr_data(&g_rl);
	g_rl.history.hist_index = g_rl.history.hist_len;
	++g_rl.history.cur_command_nb;
	if (!(buff = ft_strdup(rl_reading(&g_rl))))
		rl_err("42sh", "malloc() error", ENOMEM);
	return (buff);
}

void rl_read_handler(char *c, int fd)
{
	short nb;

	rl_set_non_canon_mode(&g_rl.non_canon_mode);
	ft_putstr_fd(tigetstr("smkx"), STDOUT_FILENO);
	nb = read(fd, c, RL_MAX_BUFF);
	if (nb < 0)
		rl_err("42sh", "read() error", NOERROR);
	c[nb] = '\0';
	ft_putstr_fd(tigetstr("rmkx"), STDOUT_FILENO);
	rl_set_mode(&g_rl.start_mode);
}

/* Minimal err_exit implementation used across the project.
   Prints a brief error message (optionally with strerror) then exits.
   ctx is unused in this simple implementation but kept for compatibility. */
void err_exit(const char *name, const char *msg, const void *ctx, int err)
{
	(void)ctx;
	if (name)
		fprintf(stderr, "%s: ", name);
	if (msg)
		fprintf(stderr, "%s", msg);
	if (err != NOERROR)
	{
		fprintf(stderr, ": %s", strerror(err));
	}
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}
