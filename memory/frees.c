/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:02:13 by dlesieur          #+#    #+#             */
/*   Updated: 2025/12/19 03:41:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdlib.h"
#include "ft_glob.h"
#include "lexer.h"

void free_list(void **list, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(list[i]);
		i++;
	}
	free(list);
}

// /* generic postorder traversal where callback receives a void* node */
// void	ast_postorder_traversal_generic(void *vnode, void (*f)(void *node))
// {
//     t_ast_node	*node;
//     size_t		i;

//     if (!vnode || !f)
//         return ;
//     node = (t_ast_node *)vnode;
//     i = 0;
//     while (i < node->children.len)
//     {
//         ast_postorder_traversal_generic(vec_nd_idx(&node->children, i), f);
//         i++;
//     }
//     f(node);
// }

/* adapter that matches the old signature but delegates to generic */
// void	ast_postorder_traversal(t_ast_node *node, void (*f)(t_ast_node *node))
// {
//     if (!node || !f)
//         return ;
//     ast_postorder_traversal_generic(node, (void (*)(void *))f);
// }

// /* generic free callback that knows how to free t_ast_node */
// void	free_node_void(void *vnode)
// {
//     t_ast_node	*node;

//     if (!vnode)
//         return ;
//     node = (t_ast_node *)vnode;
//     if (node->node_type == AST_TOKEN && node->token.allocated)
//         free(node->token.start);
//     free(node->children.buff);
//     *node = (t_ast_node){};
// }

// /* public API to free an AST root using the generic traversal + free callback */
// void	free_ast(void *root)
// {
//     if (!root)
//         return ;
//     ast_postorder_traversal_generic(root, free_node_void);
// }
