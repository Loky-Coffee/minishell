/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 21:41:32 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/08 11:56:22 by nmihaile         ###   ########.fr       */
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

	// printf("(*node)->right: |%s|", (*node)->right->tokens[0]->str);
	// printf("curr: |%s|", curr->tokens[0]->str);
	// printf("next: |%s|", next->tokens[0]->str);
	// printf("(*node): |%s|", (*node)->tokens[0]->str);

	buff = (*node)->right;
	(*node)->right = curr;
	curr->right = next;
	next->right = buff;
	(*node) = next;
	// printf("next->right: |%s|", next->right->tokens[0]->str);
}
