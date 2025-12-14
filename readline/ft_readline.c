/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:16:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 02:51:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

static char	*rl_reading(t_readline *rl)
{
	char	ch;
	char	seq1;
	char	seq2;

	rl_start_cord_data(&rl->cord);
	/* Ensure we start browsing history from the end */
	rl->history.hist_index = rl->history.hist_len;
	/* make sure line buffer is empty for new input */
	ft_str_clear(rl->line.buffer);
	rl->cord.pos = 0;

	/* enter raw mode */
	rl_set_non_canon_mode(&g_rl.non_canon_mode);
	while (read(STDIN_FILENO, &ch, 1) > 0)
	{
		/* Enter / Return -> finish input */
		if (ch == '\r' || ch == '\n')
		{
			ft_putchar('\n');
			break;
		}
		/* Ctrl-D -> EOF */
		if (ch == 0x04)
			break;
		/* Escape sequences (arrows, etc.) */
		if (ch == '\x1b')
		{
			if (read(STDIN_FILENO, &seq1, 1) <= 0)
				continue;
			if (read(STDIN_FILENO, &seq2, 1) <= 0)
				continue;
			if (seq1 != '[' && seq1 != 'O')
				continue;
			if (seq2 == 'A') /* Up */
				rl_k_up(rl);
			else if (seq2 == 'B') /* Down */
				rl_k_down(rl);
			else if (seq2 == 'C') /* Right */
				rl_k_right(rl);
			else if (seq2 == 'D') /* Left */
				rl_k_left(rl);
			continue;
		}
		/* Backspace */
		if (ch == 127 || ch == '\b')
		{
			rl_k_bcsp(rl);
			continue;
		}
		/* Printable characters -> hand off to rl_print_symb so cursor/print logic is reused */
		if (ft_isprint(ch))
		{
			char s[2] = { ch, '\0' };
			rl_print_symb(s, rl);
			continue;
		}
	}
	/* restore mode and return a heap copy of the line buffer */
	rl_set_mode(&g_rl.start_mode);
	return (ft_strdup(rl->line.buffer));
}

char *ft_readline(char *prompt)
{
	char *buff;
	char *mode;
	char fallback_buf[LINE_SIZE];

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

	/* Try the custom readline loop first. If it returns an empty string
	   (or NULL) fall back to a standard blocking fgets so the demo can
	   accept input when terminal/terminfo stubs prevent normal operation. */
	buff = NULL;
	if ((buff = ft_strdup(rl_reading(&g_rl))))
	{
		/* if the custom engine returned an empty line, use fallback */
		if (buff[0] == '\0')
		{
			free(buff);
			buff = NULL;
		}
	}
	/* Fallback: simple fgets-based input (preserve prompt already printed) */
	if (!buff)
	{
		/* restore normal cooked mode while using fgets to ensure input works */
		rl_set_mode(&g_rl.start_mode);
		if (fgets(fallback_buf, sizeof(fallback_buf), stdin))
		{
			size_t len = ft_strlen(fallback_buf);
			if (len && fallback_buf[len - 1] == '\n')
				fallback_buf[len - 1] = '\0';
			buff = ft_strdup(fallback_buf);
			if (!buff)
				rl_err("42sh", "malloc() error", ENOMEM);
		}
		else
		{
			/* EOF or error */
			buff = ft_strdup("");
			if (!buff)
				rl_err("42sh", "malloc() error", ENOMEM);
		}
		/* re-enter saved start mode to keep cleanup consistent */
		rl_set_mode(&g_rl.start_mode);
	}
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
