/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 17:25:37 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/26 22:00:33 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_tokentype	node_is_word(t_node *node)
{
	if (node && node->tokens)
		return (tkn_is_word(node->tokens[0]));
	return (NO_TOKEN);
}

static t_tokentype	node_is_redirect(t_node *node)
{
	if (node && node->tokens)
		return (tkn_is_redirect(node->tokens[0]));
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
	{
	if (i == 2)
		*ct = (*ct)->next;
		n->tokens[1] = *ct;
	}
	*ct = (*ct)->next;
	return (n);
}



static t_node	*make_pipe(t_token **ct)
{
	if (*ct) {}
	return (NULL);
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
			{
				if (node_is_redirect(left))
					set_root_node(&node, make_pipe(&ct), left, next);
				else
					set_root_node(&node, left, next, NULL);
			}
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
