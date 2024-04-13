/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 21:41:32 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/12 19:33:25 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_right_right(t_node **node, t_node *curr, t_node *next)
{
	t_node	*buff;

	buff = (*node)->right;
	(*node)->right = curr;
	if (next)
	{
		curr->right = next;
		next->right = buff;
	}
	else
		curr->right = buff;
	(*node) = next;
}

void	swap_up_righttoleft_right(t_node **node, t_node *curr, t_node *next)
{
	t_node	*buff;

	buff = (*node)->right;
	(*node)->right = curr;
	curr->left = buff;
	curr->right = next;
	(*node) = curr;
}

void	swap_up_left(t_node **node, t_node *curr, t_node *next)
{
	t_node	*buff;

	buff = (*node)->right;
	(*node)->right = next;
	next->left = curr;
	curr->right = buff;
	(*node) = next;
}

void	swap_dup_right(t_node **node, t_node *curr, t_node *next)
{
	t_node	*buff;

	buff = (*node)->right;
	(*node)->right = curr;
	curr->right = next;
	next->right = buff;
	(*node) = next;
}
