/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_insert3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:28:29 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/06 12:28:48 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*insert_right_get_curr(t_node *curr, t_node *next)
{
	if (next->parent)
	{
		next->parent->left = curr;
		curr->parent = next->parent;
	}
	curr->right = next;
	next->parent = curr;
	return (curr);
}

t_node	*insert_pipe(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL || node_is_logical_operator(next))
		return (parse_error(curr->tokens[0], ms), NULL);
	if (next->type == NODE_COMMAND \
	|| next->type == NODE_SUBSHELL || next->type == NODE_REDIRECT)
		return (insert_right_get_curr(curr, next));
	else if (next->type == NODE_PIPE)
	{
		if (next->left == NULL && next->right == NULL)
			return (parse_error(curr->tokens[0], ms), NULL);
		return (insert_right_get_curr(curr, next));
	}
	return (parse_error(curr->tokens[0], ms), NULL);
}

t_node	*insert_operator(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL || node_is_logical_operator(next))
		return (parse_error(curr->tokens[0], ms), NULL);
	if (next->type == NODE_COMMAND \
	|| next->type == NODE_SUBSHELL || next->type == NODE_REDIRECT)
		return (insert_tree_right(curr, next));
	else if (next->type == NODE_PIPE)
	{
		if (next->left == NULL && next->right == NULL)
			return (parse_error(curr->tokens[0], ms), NULL);
		if (next->parent)
		{
			next->parent->left = curr;
			curr->parent = next->parent;
		}
		curr->right = next;
		next->parent = curr;
		return (curr);
	}
	parse_error(curr->tokens[0], ms);
	return (NULL);
}
