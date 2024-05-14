/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils1_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:42:52 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:42:54 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

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
