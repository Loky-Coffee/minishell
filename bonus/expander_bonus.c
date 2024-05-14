/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:41:12 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:41:13 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	expand_tkn(t_token *token, t_node *node, t_ms *ms, int *node_nbr)
{
	char	expstr[FT_PATH_MAX];

	ms->i = 0;
	ms->j = 0;
	ms->do_wildcards = -1;
	ft_memset(expstr, 0, sizeof(expstr));
	if (expant_parameter(token, ms, expstr))
		return (1);
	if (ms->j != ms->i)
	{
		free(token->str);
		token->str = ft_calloc((ms->j + 1), sizeof(char));
		if (token->str == NULL)
			return (1);
	}
	if (expstr[0] != '\0')
		ft_strlcpy(token->str, expstr, ms->j + 1);
	if (token && token->str[0] == '\0' && token->next)
	{
		delete_empty_pre_tokens(token, node, ms, node_nbr);
		ms->do_wildcards = 0;
	}
	if (expand_wildcard_after_var(ms, token, node))
		return (1);
	return (0);
}

int	expand_singel_star_wildcard(t_node *node)
{
	int	i;

	i = 0;
	while (node->tokens[i])
	{
		if (expand_wildcard(node->tokens[i]))
		{
			if (word_splitting(node->tokens[i], node->tokens[i], \
			node->tokens[i]->next) == 1)
				return (1);
			if (reallocate_node_tokens(node, node->tokens[i], i + 1))
				return (1);
		}
		i++;
	}
	return (i);
}

int	ft_node_tokens_len(t_token *token)
{
	int		i;
	t_token	*buff;

	buff = token;
	i = 0;
	while (buff)
	{
		buff = buff->next;
		i++;
	}
	return (i);
}

int	expand_node_utils_5(t_node *node, int i)
{
	if (expand_wildcard(node->tokens[i]) > 1)
	{
		if (node->tokens[i] && node->tokens[i]->next && \
		word_splitting(node->tokens[i - 1], \
		node->tokens[i], node->tokens[i]->next) == 1)
			return (1);
		reallocate_node_tokens(node, node->tokens[i], i + 1);
	}
	return (0);
}

int	expand_node(t_node *node, t_ms *ms)
{
	int		i;

	i = 0;
	if (node == NULL || node->tokens == NULL)
		return (1);
	while (node && node->tokens && node->tokens[i] \
	&& i <= ft_node_tokens_len(*node->tokens))
	{
		if (node->tokens[0]->str[0] == '*')
		{
			if (node->tokens[0]->str[1] == '\0')
				expand_singel_star_wildcard(node);
			return (0);
		}
		if (node->tokens[i] && expand_wildcard(node->tokens[i]))
			if (expand_node_utils_5(node, i))
				return (1);
		if (node->tokens[i] && expand_tkn(node->tokens[i], node, ms, &i) == 1)
			return (1);
		i++;
	}
	return (0);
}
