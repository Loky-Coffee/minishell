/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 16:33:05 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/06 20:40:14 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_nodetype	node_is_pipe(t_node *node)
{
	if (node && node->type == NODE_PIPE)
		return (NODE_PIPE);
	return (NO_NODE);
}

t_nodetype	node_is_redirect(t_node *node)
{
	if (node && node->type == NODE_REDIRECT)
		return (NODE_REDIRECT);
	return (NO_NODE);
}

t_nodetype	node_is_logical_operator(t_node *node)
{
	if (node && (node->type == NODE_AND || node->type == NODE_OR))
		return (node->type);
	return (NO_NODE);
}
