/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:56:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/12 16:26:47 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_node	*parse_leaf(t_token **ct)
{
	t_node	*n;

	if (*ct == NULL)
		return (NULL);
	n = NULL;
	if (tkn_is_word(*ct))
		n = make_word(ct);
	else if (tkn_is_redirect(*ct))
		n = make_redirect(ct);
	else if (tkn_is_operator(*ct))
		n = make_operator(ct);
	return (n);
}

t_node	*add_cmd_to_redirect(t_node *cmd, t_node *redirect)
{
	
}

t_node	*insert_cmd(t_node *curr, t_node *next)
{
	t_node	*buff;
	
	// if (next == NULL)
	// 	return (curr);
	if (next->type == NODE_REDIRECT)
	{
		buff = next;
		while (next->left && next->left->type == NODE_REDIRECT)
			next = next->left;
		next->left = curr;
		curr->parent = next;
		return (buff);
	}
	if (next->type == NODE_PIPE)
	{
		if (next->left == NULL)
		{
			next->left = curr;
			curr->parent = next;
			return (next);
		}
		else if (next->left == NODE_REDIRECT)
		{
			i
		}
			; // is it inavlid ???
	}
	if (next->type == NODE_AND || next->type == NODE_OR)
	{
		
	}
	
}

t_node	*ft_parse(t_token *ct, t_node **root)
{
	t_node	*curr;
	t_node	*next;
	t_node	*ast;

	curr = parse_leaf(&ct);
	if (curr == NULL)
		return (NULL);
	next = ft_parse(ct, root);
	if (next == NULL)
	{
		if (*root == NULL)
			*root = curr;
		return (curr);
	}
	if (*root && (*root)->type <= curr->type)
		*root = curr;
	if (curr->type == NODE_COMMAND)
		ast = insert_cmd(curr, next);
	else if (curr->type == NODE_REDIRECT)
		ast = insert_redirect(curr, next);
	else if (curr->type == NODE_PIPE)
		ast = insert_pipe(curr, next);
	else if (curr->type == NODE_AND || curr->type == NODE_OR)
		ast = insert_operator(curr, next);
	else
		return (curr);
	return (ast);
}
