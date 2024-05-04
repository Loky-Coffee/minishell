/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_insert2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 19:29:18 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/04 19:33:26 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_node	*push_redirectout_into_place(t_node *curr, t_node *next)
{
	t_node	*buff;

	buff = next;
	if (curr->tokens[0]->type == TOKEN_GREATER \
	|| curr->tokens[0]->type == TOKEN_DGREATER)
		while (next->left && (next->left->tokens[0]->type == TOKEN_LESS \
		|| next->left->tokens[0]->type == TOKEN_DLESS))
			next = next->left;
	if (curr->tokens[0]->type == TOKEN_LESS \
	|| curr->tokens[0]->type == TOKEN_DLESS \
	|| (tkn_is_redirect_out(curr->tokens[0]) \
	&& tkn_is_redirect_out(next->tokens[0])))
	{
		if (next->parent)
		{
			curr->parent = next->parent;
			next->parent->left = curr;
		}
		curr->left = next;
		next->parent = curr;
		return (curr);
	}
	else
	{
		if (next->left)
		{
			next->left->parent = curr;
			curr->left = next->left;
		}
		next->left = curr;
		curr->parent = next;
	}
	return (buff);
}

t_node	*insert_redirect(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL)
		return (curr);
	if (next->type == NODE_COMMAND || next->type == NODE_SUBSHELL)
	{
		if (next->parent)
		{
			curr->parent = next->parent;
			next->parent->left = curr;
		}
		curr->left = next;
		next->parent = curr;
		return (curr);
	}
	else if (next->type == NODE_REDIRECT)
		return (push_redirectout_into_place(curr, next));
	else if (next->type == NODE_PIPE \
	|| next->type == NODE_AND || next->type == NODE_OR)
	{
		if (next->left == NULL || next->left->type == NODE_COMMAND || \
		next->left->type == NODE_SUBSHELL || next->left->type == NODE_REDIRECT)
			return (insert_node_left(curr, next));
		else
			parse_error(curr->tokens[0], ms);
	}
	return (NULL);
}

t_node	*insert_pipe(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL || node_is_logical_operator(next))
		return (parse_error(curr->tokens[0], ms), NULL);
	if (next->type == NODE_COMMAND \
	|| next->type == NODE_SUBSHELL || next->type == NODE_REDIRECT)
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
