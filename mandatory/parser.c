/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:56:24 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/20 20:18:08 by nmihaile         ###   ########.fr       */
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

t_node	*insert_cmd_to_redirect(t_node *curr, t_node *next, t_ms *ms)
{
	t_node	*buff;
	t_token	**tkn_buff;
	int		i[5];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	i[3] = 0;
	i[4] = 0;
	buff = next;
	while (next->left && next->left->type == NODE_REDIRECT)
		next = next->left;
	if (next->left && (next->left->type == NODE_COMMAND \
	|| next->left->type == NODE_SUBSHELL))
	{
		while (curr->tokens[i[0]])
			i[0]++;
		while (next->left->tokens[i[1]])
			i[1]++;
		tkn_buff = ft_calloc(i[0] + i[1] + 1, sizeof(t_token *));
		if (tkn_buff == NULL)
			parse_error(curr->tokens[0], ms);
		while (curr->tokens[i[2]])
		{
			tkn_buff[i[4]] = curr->tokens[i[2]];
			i[2]++;
			i[4]++;
		}
		while (next->left->tokens[i[3]])
		{
			tkn_buff[i[4]] = next->left->tokens[i[3]];
			i[3]++;
			i[4]++;
		}
		free(next->left);
		free(curr->tokens);
		curr->tokens = tkn_buff;
	}
	next->left = curr;
	curr->parent = next;
	return (buff);
}

t_node *insert_node_left(t_node *curr, t_node *next)
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

void	parse_error(t_token *tkn, t_ms *ms)
{
	ms->parse_error = 1;
	ms->parse_errtkn = tkn;
}

t_node	*insert_cmd(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL) // chek if necessary
		return (curr);
	if (node_is_redirect(next))
		return (insert_cmd_to_redirect(curr, next, ms));
	if (node_is_pipe(next))
	{
		if (next->left == NULL)
		{
			next->left = curr;
			curr->parent = next;
			return (next);
		}
		else if (next->left->type == NODE_REDIRECT)
			return (insert_cmd_to_redirect(curr, next, ms));
		else
			parse_error(curr->tokens[0], ms);
	}
	else if (next->type == NODE_AND || next->type == NODE_OR)
	{
		if (next->left == NULL)
		{
			next->left = curr;
			curr->parent = next;
			return (curr);
		}
		else if (next->left && next->left->type == NODE_REDIRECT)
		{
			return (insert_cmd_to_redirect(curr, next, ms));
		}
		else
			parse_error(curr->tokens[0], ms);
	}
	else
		parse_error(curr->tokens[0], ms);
	return (NULL);
}

t_node	*insert_redirect(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL)
		return (curr);
	if (next->type == NODE_COMMAND || next->type == NODE_SUBSHELL || next->type == NODE_REDIRECT) // || next->type == NODE_SUBSHELL
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
	else if (next->type == NODE_PIPE || next->type == NODE_AND || next->type == NODE_OR)
	{
		if (next->left == NULL || next->left->type == NODE_COMMAND || next->left->type == NODE_SUBSHELL || next->left->type == NODE_REDIRECT)
			return (insert_node_left(curr, next));
		else
			parse_error(curr->tokens[0], ms);
	}
	return (NULL);
}

t_node	*insert_pipe(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL || node_is_andor(next))
		return (parse_error(curr->tokens[0], ms), NULL);
	if (next->type == NODE_COMMAND || next->type == NODE_SUBSHELL || next->type == NODE_REDIRECT) // || next->type == NODE_SUBSHELL
	{
		if (next->parent)
		{
			next->parent->left = curr;
			curr->parent = next->parent;
		}
		curr->right = next;
		next->parent = curr;
		return (curr);
	}
	else if (next->type == NODE_PIPE)
	{
		if (next->left == NULL && next->right == NULL)
			return (parse_error(curr->tokens[0], ms), NULL);
		if (next->parent)
		{
			next->parent->left = curr;
			curr->parent = next->parent;
		}
		curr->right = next;
		next->parent = curr;
		return (curr);
	}
	return (parse_error(curr->tokens[0], ms), NULL);
}

t_node	*insert_operator(t_node *curr, t_node *next, t_ms *ms)
{
	if (next == NULL || node_is_andor(next))
		return (parse_error(curr->tokens[0], ms), NULL);
	if (next->type == NODE_COMMAND || next->type == NODE_SUBSHELL || next->type == NODE_REDIRECT) // next->type == || NODE_SUBSHELL
		return (insert_tree_right(curr, next));
	else if (next->type == NODE_PIPE)
	{
		if (next->left == NULL && next->right == NULL)
			return (parse_error(curr->tokens[0], ms), NULL);
		if (next->parent)
		{
			next->parent->left = curr;
			curr->parent = next->parent;
		}
		curr->right = next;
		next->parent = curr;
		return (curr);
	}
	parse_error(curr->tokens[0], ms);
	return (NULL);
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
	if (next == NULL)
	{
		if (*root == NULL)
			*root = curr;
		if (curr->type == NODE_PIPE || curr->type == NODE_AND || curr->type == NODE_OR)
			parse_error(curr->tokens[0], ms);
		return (curr);
	}
	if (*root && (*root)->type <= curr->type && (*root)->type <= NODE_PIPE)
		*root = curr;
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
