/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:33:05 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/03 10:52:44 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_nodetype	node_is_pipe(t_node *node)
{
	if (node && node->type == NODE_PIPE)
		return (NODE_PIPE);
	return (NO_NODE);
	// if (node && node->tokens && node->tokens[0]
	// 	&& node->tokens[0]->type == TOKEN_PIPE)
	// 	return (TOKEN_PIPE);
	// return (NO_TOKEN);
}

t_nodetype	node_is_redirect(t_node *node)
{
	if (node && node->type == NODE_REDIRECT)
		return (NODE_REDIRECT);
	return (NO_NODE);
	// if (node && node->tokens)
	// 	return (tkn_is_redirect(node->tokens[0]));
	// return (NO_TOKEN);
}

t_nodetype	node_is_logical_operator(t_node *node)
{
	if (node && (node->type == NODE_AND || node->type == NODE_OR))
		return (node->type);
	return (NO_NODE);
	// if (node && node->tokens && node->tokens[0]->type == TOKEN_DAND)
	// 	return (TOKEN_DAND);
	// else if (node && node->tokens && node->tokens[0]->type == TOKEN_OR)
	// 	return (TOKEN_OR);
	// return (NO_TOKEN);
}
