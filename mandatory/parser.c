/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:25:37 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/20 16:19:53 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	get_same_token_len(t_token *token)
{
	t_token	*t;
	int		len;

	t = token;
	len = 0;
	if (t && t->next && t->type != t->next->type)
		return (1);
	while (t && t->type == token->type)
	{
		t = t->next;
		len++;
	}
	return (len);
}

static t_token	*get_next_token_type(t_token *token)
{
	t_token *t;
	
	t = token->next;
	while (t && t->type == token->type)
		t = t->next;
	return (t);
}

static t_node	*create_node(t_token *token, struct s_node *left, struct s_node *right)
{
	t_node	*n;
	t_token	*t;
	int		i;

	n = (t_node *)malloc(1 * sizeof(t_node));
	if (n == NULL)
		return (NULL);
	n->left = left;
	n->right = right;
	n->tokens = ft_calloc(get_same_token_len(token), sizeof(t_token));
	if (n->tokens == NULL)
			// @TODO clear node.
			return (NULL);
	t = token;
	i = 0;
	while (t && t->type == token->type)
	{
		n->tokens[i] = t;
		t = t->next;
		i++;
	}
	return (n);
}

void	ft_parse2(t_token *current_token, t_node **current_node)
{
	t_token	*next_token;
	t_node	*new_node;
	t_node	*next_node;

	if (current_token == NULL)
		return ;
	next_token = current_token->next;
	while (next_token && next_token->type == current_token->type)
		next_token = next_token->next;
	
	new_node = create_node(current_token, NULL, NULL);
	next_node = NULL;
	if (next_token && is_operator(next_token->content))
		next_node = create_node(next_token, NULL, NULL);

	if (current_token && is_operator(current_token->content))
	{
		// we are a operator and have to do diff stuff for | and && and ||
	}
	else
	{
		if (current_node && *current_node)
		{
			if (next_node)
			{
				(*current_node)->right = next_node;
				next_node->left = new_node;
				ft_parse2(next_token->next, &next_node->right);
			}
			else
			{
				(*current_node)->right = new_node;
				ft_parse2(get_next_token_type(current_token), &new_node->right);
			}
		}
		else
		{
			if (next_node)
			{
				*current_node = next_node;
				(*current_node)->left = new_node;
				ft_parse2(next_token->next, &(*current_node)->right);
			}
			else
			{
				*current_node = new_node;
				ft_parse2(get_next_token_type(current_token), &(*current_node)->right);
			}
		}
	}
}
