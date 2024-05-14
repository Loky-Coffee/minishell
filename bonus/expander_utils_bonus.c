/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:41:24 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:41:26 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

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
		if (curr->next)
			curr = curr->next;
		i++;
	}
	if (node->tokens)
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
	node->tokens[i], node->tokens[i]->next) == 1)
		return (1);
	if (reallocate_node_tokens(node, node->tokens[0], i - oi + 1))
		return (1);
	return (0);
}

void	delete_empty_pre_tokens(t_token *token, \
t_node *node, t_ms *ms, int *node_nbr)
{
	int	a_flag[2];

	set_zero(&a_flag[0], &a_flag[1]);
	while (node && node->tokens && node->tokens[a_flag[0]]->str != 0)
	{
		if (node->tokens[a_flag[0]] == token)
		{
			while (node && node->tokens[a_flag[0]])
			{
				node->tokens[a_flag[0]] = node->tokens[a_flag[0] + 1];
				a_flag[0]++;
				a_flag[1] = 1;
			}
			node->tokens[a_flag[0]] = NULL;
			break ;
		}
		a_flag[0]++;
	}
	(*node_nbr) = (*node_nbr);
	if (token->next && tkn_is_operator(token->next) == NO_TOKEN)
	{
		if (a_flag[1] == 0)
			(*node_nbr)++;
		expand_tkn(token->next, node, ms, node_nbr);
	}
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
			if (reallocate_node_tokens(node, token, count - 1))
				return (1);
		}
		if (expand_wildcard(token))
			split_and_reallocate_node(node);
	}
	return (0);
}

void	free_token_and_set(t_token **token, t_token *buff)
{
	free((*token)->str);
	free((*token));
	*token = buff;
}
