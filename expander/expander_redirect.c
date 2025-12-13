/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 14:59:25 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/13 15:56:58 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "lexer.h"
#include "ft_glob.h"
#include "expander.h"
#include "executor.h"

bool	create_redir(t_token_type tt, char *fname, t_redir *ret)
{
	ft_assert(tt != TOKEN_REDIR_HEREDOC && "HEREDOCS are handled separately");
	ret->fname = fname;
	ret->direction_in = tt == TOKEN_REDIR_FD_IN;
	if (!ret->fname)
		ret->fd = open(ret->fname, O_RDONLY);
	if (tt == TOKEN_REDIR_FD_IN)
		ret->fd = open(ret->fname, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else if (tt == TOKEN_REDIR_OUT)
		ret->fd = open(ret->fname, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (ret->fd < 0)
		return (false);
	ret->should_delete = false;
	return (true);
}

void print_redir_err(char **context, t_tok full_token, char *expanded_name)
{
	if (!expanded_name)
		ft_eprintf("%s: %.*s: ambigous redirect\n", *context, full_token.len, full_token.start);
	else
		ft_eprintf("%s: %s: %s\n", *context, expanded_name, strerror(errno));
}

t_tok	get_old_token(t_ast_node word)
{
	t_tok	ret;

	ft_assert(word.node_type == AST_WORD);
	ft_assert(word.children.len);
	ret = word.children.buff[0].token.full_word;
	ft_assert(ret.present);
	return (ret);
}

int	redirect_from_ast_redir(char **context, t_ast_node *curr, int *redir_idx)
{
	t_redir	new_redir;
	t_token_type	tt;
	t_tok			full_token;
	char			*fname;

	ft_assert(curr->node_type == AST_REDIRECT);
	if (curr->has_redirect)
	{
		*redir_idx = curr->redir_idx;
		return (0);
	}
	tt = curr->children.buff[0].token.tt;
	full_token = get_old_token(curr->children.buff[1]);
	fname = expand_word_single(, vec_idx(&curr->children, 1));
	if (!create_redir(tt, fname, &new_redir))
	{
		print_redir_err(context, full_token, fname);
		free(fname);
		return (-1);
	}
	curr->redir_idx = redirects.len;
	*redir_idx = redirects.len;
	curr->has_redirect = true;
	vec_push(redirects, new_redir);
	return (0);
}