/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:25:37 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/08 11:54:59 by nmihaile         ###   ########.fr       */
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

static void	set_root_node(t_node **root, t_node *node, t_node *rgt)
{
	if (node && node_is_redirect(node))
	{
		node->right = rgt;
		(*root) = node;
	}
	else
	{
		node->left = rgt;
		(*root) = node;
	}
}

static void	set_node_left(t_node **node, t_node *curr, t_node *next)
{
	if (next && !node_is_word(next))
	{
		(*node)->left = next;
		(*node)->right = curr;
		*node = curr;
	}
	else
	{
		(*node)->left = curr;
		(*node)->right = next;
		*node = next;
	}
}

static void	set_node_right(t_node **node, t_node *curr, t_node *next)
{
	if (curr && node_is_redirect(curr))
	{
		(*node)->right = curr;
		curr->right = next;
	}
	else
	{
		(*node)->right = curr;
		curr->left = next;
	}
	*node = curr;
}

// static void	flip_add_right_right(t_node **node, t_node *curr, t_node *next)
// {
// 	(*node)->right = next;
// 	next->right = curr;
// 	(*node) = curr;
// }

// static void	add_left_right(t_node **node, t_node *curr, t_node *next)
// {
// 	(*node)->left = curr;
// 	(*node)->right = next;
// 	(*node) = next;
// }

int	ft_parse(t_token *ct, t_node **cn)
{
	t_node	*curr;
	t_node	*next;
	t_node	*node;
	// t_node	*buff;

	node = *cn;
	while (ct)
	{
		curr = parse_leaf(&ct);
		if (ct == NULL)
			next = NULL;
		else
			next = parse_leaf(&ct);
		if (*cn == NULL)
		{
			if (curr && node_is_redirect(curr) && next && node_is_redirect(next))
			{
				set_root_node(&node, curr, next);
				*cn = curr;
				node = next;
			}
			else if (next && !node_is_word(next) && !node_is_redirect(curr))
				set_root_node(&node, next, curr);
			else
					set_root_node(&node, curr, next);
			if (*cn == NULL)
				*cn = node;
		}
		else
		{
			if (node_is_redirect(curr) && node_is_word(next))
				set_node_right(&node, curr, next);
				// add_right_right(&node, curr, next);
			else if (!node_is_redirect(node) && (node_is_redirect(curr) && node_is_redirect(next)))
				// add_right_right(&node, curr, next);
				swap_dup_right(&node, curr, next);
			else if ( \
			(node_is_redirect(curr) && node_is_redirect(next)) \
			|| (node_is_word(curr) && next == NULL) \
			|| (node_is_redirect(curr)))
				add_right_right(&node, curr, next);
			else if (node_is_word(curr) && next && node_is_redirect(next))
			{
				node->right = next;
				next->right = curr;
				node = node->right;
			}
			else if (node_is_pipe(curr) && node_is_word(next))
				swap_up_righttoleft_right(&node, curr, next);
			else if (node_is_word(curr) && node_is_pipe(next))
				swap_up_left(&node, curr, next);
			else if (node->left == NULL && !node_is_redirect(node))
				set_node_left(&node, curr, next);
			else
				set_node_right(&node, curr, next);
		}
	}
	return (0);
}
