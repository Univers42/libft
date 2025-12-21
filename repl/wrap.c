/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 23:36:12 by marvin            #+#    #+#             */
/*   Updated: 2025/12/20 23:36:12 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_readline.h"
#include <stdbool.h>
#include "ft_stdio.h"
#include "ipc.h"
#include "var.h"
#include "ipc.h"
#include "lexer.h"
#include <ctype.h> 
#include "ft_wctype.h" 
#include <signal.h> 
#include "trap.h"

/* Wrap ANSI CSI/escape sequences with readline
non-printing markers \001...\002
   If the prompt already contains \001 markers, do nothing.
   Wrapping will only be applied if the config requests
   it (wrap_prompt_nonprint). */
char	*wrap_nonprint_if_enabled(const char *s)
{
	t_repl_config	*conf;

	conf = get_repl_config();
	if (conf && !conf->wrap_prompt_nonprint)
		return (ft_strdup(s));
	return (wrap_nonprint(s));
}

char	*wrap_combining_if_enabled(const char *s)
{
	t_repl_config	*conf;

	conf = get_repl_config();
	if (conf && !conf->enable_unicode_prompt)
		return (ft_strdup(s));
	return (wrap_combining(s));
}
