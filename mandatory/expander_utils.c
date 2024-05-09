/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 07:16:35 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 10:36:56 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	reallocate_node_tokens(t_node *node, t_token *curr, int i)
{
	int		count;
	t_token	**tokens;

	count = i + 1;
	tokens = (t_token **)ft_calloc(count + 1, sizeof(t_token *));
	if (tokens == NULL)
		return (1);
	i = 0;
	while (i < count && curr)
	{
		tokens[i] = curr;
		curr = curr->next;
		i++;
	}
	free(node->tokens);
	node->tokens = tokens;
	return (0);
}

int	split_and_reallocate_node(t_node *node)
{
	int		i;
	int		oi;

	i = 1;
	oi = i;
	if (node->tokens[i] == NULL)
		return (0);
	if (word_splitting(node->tokens[i - 1], \
	node->tokens[i], node->tokens[i]->next, &i) == 1)
		return (1);
	if (reallocate_node_tokens(node, node->tokens[0], i - oi + 1))
		return (1);
	return (0);
}

void	delete_empty_pre_tokens(t_token *token, \
t_node *node, t_ms *ms, int *node_nbr)
{
	int		a;

	a = 0;
	while (node && node->tokens && node->tokens[a] && node->tokens[a]->str != 0)
	{
		if (node->tokens[a] == token)
		{
			while (node && node->tokens[a])
			{
				node->tokens[a] = node->tokens[a + 1];
				a++;
			}
			node->tokens[a] = NULL;
			break ;
		}
		a++;
	}
	(*node_nbr)++;
	if (token->next)
		expand_tkn(token->next, node, ms, node_nbr);
}

int	expand_wildcard_after_var(t_ms *ms, t_token *token, t_node *node)
{
	int	count;

	count = 0;
	if (ms->do_wildcards == 1 && node->tokens[0]->type != TOKEN_TLESS)
	{
		if (token == node->tokens[0])
		{
			word_split_token(&token, ms, &count, NULL);
			if (reallocate_node_tokens(node, token, count + 1))
				return (1);
		}
		if (expand_wildcard(token))
			split_and_reallocate_node(node);
	}
	return (0);
}
