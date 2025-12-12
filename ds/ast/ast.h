/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 02:41:32 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/12 02:44:17 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdbool.h>

typedef enum e_ast_node
{
	AST_NODE_UNKNOWN = 0,
	AST_NODE_TOKEN,
	AST_NODE_LIST
}t_ast_node_t;

typedef struct s_ast_node
{
	t_ast_node_t    type;
	void            *payload;		// language specific data
	void            *meta;			// optional toekn/pos/annotations
	size_t			n_children;
	size_t			cap_children;
	struct s_ast_node	**children;
}t_ast_node;
#endif
