/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rl_term_mode.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 01:19:02 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/14 01:19:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "x_readline.h"

void		rl_save_mode(struct termios *savetty)
{
	if (tcgetattr(STDIN_FILENO, savetty) == -1)
	{
		ft_putendl_fd("42sh: tcgetattr() error", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

void		rl_set_mode(struct termios *savetty)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, savetty) == -1)
	{
		ft_putendl_fd("42sh: tcssetattr() error", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

void		rl_set_non_canon_mode(struct termios *tty)
{
	rl_save_mode(tty);
	tty->c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
	tty->c_cc[VMIN] = 1;
	tty->c_cc[VTIME] = 0;
	rl_set_mode(tty);
}
