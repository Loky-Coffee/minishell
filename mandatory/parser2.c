/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 14:58:11 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/11 17:22:00 by aalatzas         ###   ########.fr       */
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

t_node	*ft_parse(t_token *ct, t_node **root)
{
	t_node	*curr;
	t_node	*next;

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
	if (next->parent && next->parent->parent && curr->type > next->parent->type)
	{
		next->parent->parent->left = curr;
		curr->right = next->parent;
		curr->parent = next->parent->parent;
		next->parent = curr;
	}
	else if (next->parent && next->parent->parent == NULL && curr->type > next->parent->type)
	{
		next->parent->parent->right = curr;
		curr->right = next->parent;
		curr->parent = next->parent->parent;
		next->parent = curr;
	}
	else if (curr->type > next->type)
	{
		if (next->right == NULL && next->parent && next->type < curr->type && next->parent->type != curr->type)
		{
			next->parent->left = curr;
			curr->parent = next->parent;
		}
		else if (curr->type > next->type && next->parent && next->parent->parent)
		{
			next->parent->parent->left = curr;
			curr->parent = next->parent->parent;
			curr->right = next->parent;
			next->parent = curr;
			return (curr);
		}
		curr->right = next;
		next->parent = curr;
		return (curr);
	}
	else if (next->type == curr->type && next->left)
	{
		curr->right = next;
		next->parent = curr;
		return(curr);
	}
	else if (next->type >= curr->type && next->left == NULL)
	{
		next->left = curr;
		curr->parent = next;
		if (curr->type < next->type && (*root)->type <= NODE_PIPE)
			return (next);
		return (curr);
	}
	return (curr);
}
