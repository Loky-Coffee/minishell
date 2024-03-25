/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:25:37 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/25 17:25:39 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_tokentype	node_is_word(t_node *node)
{
	if (node && node->tokens)
		return (tkn_is_word(node->tokens[0]));
	return (NO_TOKEN);
}

static int	create_node(t_token **token, t_node **node)
{
	t_node	*n;
	
	if (*token == NULL)
		return (1);
	n = (t_node *)ft_calloc(1, sizeof(t_node));
	if (n == NULL)
		return (1);
	*node = n;
	return (0);
}

static t_node	*make_word(t_token **ct)
{
	t_node	*n;
	t_token	*t;
	int		i;
	
	if (create_node(ct, &n))
		return (NULL);
	n->left = NULL;
	n->right = NULL;
	t = *ct;
	i = 0;
	while (t && !tkn_is_operator(t) && !tkn_is_redirect(t))
	{
		i++;
		t = t->next;
	}
	n->tokens = (t_token **)ft_calloc(i + 1, sizeof(t_token));
	if (n->tokens == NULL)
	{
		free(n);
		return (NULL);
	}
	i = 0;
	while (*ct && !tkn_is_operator(*ct) && !tkn_is_redirect(*ct))
	{
		n->tokens[i++] = *ct;
		*ct = (*ct)->next;
	}
	return (n);
}

static t_node	*make_operator(t_token **ct)
{
	t_node *n;

	if (create_node(ct, &n))
		return (NULL);
	n->tokens = (t_token **)ft_calloc(2, sizeof(t_token));
	if (n->tokens == NULL)
	{
		free(n);
		return (NULL);
	}
	n->left = NULL;
	n->right = NULL;
	n->tokens[0] = *ct;
	*ct = (*ct)->next;
	return (n);
}

static t_node	*make_redirect(t_token **ct)
{
	int		i;
	t_node	*n;

	if (create_node(ct, &n))
		return (NULL);
	i = 1;
	if ((*ct)->next)
		i++;
	n->tokens = (t_token **)ft_calloc(i + 1, sizeof(t_token));
	if (n->tokens == NULL)
	{
		free(n);
		return (NULL);
	}
	n->left = NULL;
	n->right = NULL;
	n->tokens[0] = *ct;
	if (i == 2)
	{
		*ct = (*ct)->next;
		n->tokens[1] = *ct;
	}
	*ct = (*ct)->next;
	return (n);
}

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

static void	set_root_node(t_node **root, t_node *node, t_node *lft, t_node *rgt)
{
	node->left = lft;
	node->right = rgt;
	(*root) = node;
}

static void	set_node_left(t_node **node, t_node *left, t_node *next)
{
	if (next && !node_is_word(next))
	{
		(*node)->left = next;
		(*node)->right = left;
		*node = left;
	}
	else
	{
		(*node)->left = left;
		(*node)->right = next;
		*node = next;
	}							
}

static void	set_node_right(t_node **node, t_node *left, t_node *next)
{
	if (next && !node_is_word(next))
	{
		(*node)->right = next;
		next->left = left;
		*node = next;
	}
	else
	{
		(*node)->right = left;
		left->left = next;
		*node = left;
	}
}

int	ft_parse(t_token *ct, t_node **cn)
{
	t_node	*left;
	t_node	*next;
	t_node	*node;

	node = *cn;
	while (ct)
	{
		left = parse_leaf(&ct);
		if (ct == NULL)
			next = NULL;
		else
			next = parse_leaf(&ct);
		if (*cn == NULL)
		{
			if (next && !node_is_word(next))
				set_root_node(&node, next, left, NULL);
			else
				set_root_node(&node, left, next, NULL);
			*cn = node;
		}
		else
		{
			if (node->left == NULL)
				set_node_left(&node, left, next);
			else
				set_node_right(&node, left, next);
		}
	}
	return (0);
}


















// static int	get_same_token_len(t_token *token)
// {
// 	t_token	*t;
// 	int		len;

// 	t = token;
// 	len = 0;
// 	if (t && t->next && t->type != t->next->type)
// 		return (1);
// 	while (t && t->type == token->type)
// 	{
// 		t = t->next;
// 		len++;
// 	}
// 	return (len);
// }

// static t_token	*get_next_token_type(t_token *token)
// {
// 	t_token *t;

// 	t = token->next;
// 	while (t && t->type == token->type)
// 		t = t->next;
// 	return (t);
// }

// static t_node	*create_node(t_token *token, struct s_node *left, struct s_node *right)
// {
// 	t_node	*n;
// 	t_token	*t;
// 	int		i;

// 	n = (t_node *)malloc(1 * sizeof(t_node));
// 	if (n == NULL)
// 		return (NULL);
// 	n->left = left;
// 	n->right = right;
// 	n->tokens = ft_calloc(get_same_token_len(token), sizeof(t_token));
// 	if (n->tokens == NULL)
// 			// @TODO clear node.
// 			return (NULL);
// 	t = token;
// 	i = 0;
// 	while (t && t->type == token->type)
// 	{
// 		n->tokens[i] = t;
// 		t = t->next;
// 		i++;
// 	}
// 	return (n);
// }

// void	ft_parse(t_token *current_token, t_node **current_node)
// {
// 	t_token	*next_token;
// 	t_node	*new_node;
// 	t_node	*next_node;

// 	if (current_token == NULL)
// 		return ;
// 	next_token = current_token->next;
// 	while (next_token && next_token->type == current_token->type)
// 		next_token = next_token->next;

// 	new_node = create_node(current_token, NULL, NULL);
// 	next_node = NULL;
// 	if (next_token && is_operator(next_token->str))
// 		next_node = create_node(next_token, NULL, NULL);

// 	if (current_token && is_operator(current_token->str))
// 	{
// 		// we are a operator and have to do diff stuff for | and && and || and < and > and << and >>
// 		if (current_token->type == TOKEN_LESS)
// 		{
			
// 		}
// 	}
// 	else
// 	{
// 		if (current_node && *current_node)
// 		{
// 			if (next_node)
// 			{
// 				(*current_node)->right = next_node;
// 				next_node->left = new_node;
// 				ft_parse(next_token->next, &next_node->right);
// 			}
// 			else
// 			{
// 				(*current_node)->right = new_node;
// 				ft_parse(get_next_token_type(current_token), &new_node->right);
// 			}
// 		}
// 		else
// 		{
// 			if (next_node)
// 			{
// 				*current_node = next_node;
// 				(*current_node)->left = new_node;
// 				ft_parse(next_token->next, &(*current_node)->right);
// 			}
// 			else
// 			{
// 				*current_node = new_node;
// 				ft_parse(get_next_token_type(current_token), &(*current_node)->right);
// 			}
// 		}
// 	}
// }
