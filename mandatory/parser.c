/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:56:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/06 11:17:05 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*parse_leaf(t_ms *ms, t_token **ct)
{
	t_node	*n;

	if (*ct == NULL)
		return (NULL);
	n = NULL;
	if ((*ct)->type == TOKEN_WORD)
		n = make_word(ct);
	else if ((*ct)->type == TOKEN_SUBSHELL)
		n = make_subshell(ct);
	else if (tkn_is_redirect(*ct))
		n = make_redirect(ms, ct);
	else if (tkn_is_operator(*ct))
		n = make_operator(ct);
	return (n);
}

t_node	*insert_node_left(t_node *curr, t_node *next)
{
	if (next->left == NULL)
	{
		next->left = curr;
		curr->parent = next;
	}
	else
	{
		curr->left = next->left;
		next->left->parent = curr;
		next->left = curr;
		curr->parent = next;
	}
	return (next);
}

t_node	*insert_tree_right(t_node *curr, t_node *next)
{
	if (next->parent == NULL)
	{
		curr->right = next;
		next->parent = curr;
	}
	else
	{
		curr->right = next;
		next->parent->left = curr;
		curr->parent = next->parent;
		next->parent = curr;
	}
	return (curr);
}

int	handle_root(t_node *curr, t_node *next, t_node **root, t_ms *ms)
{
	if (next == NULL)
	{
		if (*root == NULL)
			*root = curr;
		if (curr->type == NODE_PIPE \
		|| curr->type == NODE_AND || curr->type == NODE_OR)
			parse_error(curr->tokens[0], ms);
		return (1);
	}
	if (*root && (*root)->type <= curr->type && (*root)->type <= NODE_PIPE)
	{
		if ((*root)->type == NODE_REDIRECT && curr->type == NODE_REDIRECT)
		{
			if (tkn_is_redirect_in(curr->tokens[0]) \
			|| (tkn_is_redirect_out((*root)->tokens[0]) \
			&& tkn_is_redirect_out(curr->tokens[0])))
				*root = curr;
		}
		else
			*root = curr;
	}
	return (0);
}

t_node	*ft_parse(t_token *ct, t_node **root, t_ms *ms)
{
	t_node	*curr;
	t_node	*next;
	t_node	*ast;

	curr = parse_leaf(ms, &ct);
	if (curr == NULL)
		return (NULL);
	next = ft_parse(ct, root, ms);
	if (ms->parse_error == 258)
		return (free(curr), NULL);
	if (handle_root(curr, next, root, ms))
		return (curr);
	if (curr->type == NODE_COMMAND || curr->type == NODE_SUBSHELL)
		ast = insert_cmd(curr, next, ms);
	else if (curr->type == NODE_REDIRECT)
		ast = insert_redirect(curr, next, ms);
	else if (curr->type == NODE_PIPE)
		ast = insert_pipe(curr, next, ms);
	else if (curr->type == NODE_AND || curr->type == NODE_OR)
		ast = insert_operator(curr, next, ms);
	else
		return (curr);
	return (ast);
}
