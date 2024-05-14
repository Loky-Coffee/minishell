/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_insert2_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:43:20 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:43:22 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static t_node	*insert_next_left_get_curr(t_node *curr, t_node *next)
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

t_node	*push_redirectout_into_place(t_node *curr, t_node *next)
{
	t_node	*buff;

	buff = next;
	if (tkn_is_redirect_out(curr->tokens[0]))
		while (next->left && (next->left->tokens[0]->type == TOKEN_LESS \
		|| next->left->tokens[0]->type == TOKEN_DLESS))
			next = next->left;
	if (tkn_is_redirect_in(curr->tokens[0]) \
	|| (tkn_is_redirect_out(curr->tokens[0]) \
	&& tkn_is_redirect_out(next->tokens[0])))
		return (insert_next_left_get_curr(curr, next));
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
