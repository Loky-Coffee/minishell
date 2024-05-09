/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 18:15:06 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	expand_single_char(char *dst, int *j, char *src, int *i)
{
	(*i)++;
	if (src[(*i)])
		dst[(*j)++] = src[(*i)];
}

void	expand_quote(char *qm, char *dst, int *j, char *src)
{
	if (*qm == '\0')
		*qm = *src;
	else if (*qm == *src)
		*qm = '\0';
	else
		dst[(*j)++] = *src;
}

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
	if (token && token->str[0] == '\0' && token->next)
	{
		delete_empty_pre_tokens(token, node, ms, node_nbr);
		ms->do_wildcards = 0;
	}
	if (expand_wildcard_after_var(ms, token, node))
		return (1);
	return (0);
}

int	expand_node(t_node *node, t_ms *ms)
{
	int		i;
	int		oi;

	i = 0;
	if (node == NULL || node->tokens == NULL)
		return (1);
	while (node->tokens[i])
	{
			if (expand_wildcard(node->tokens[i]))
		{
			oi = i;
			if (word_splitting(node->tokens[i - 1], \
			node->tokens[i], node->tokens[i]->next, &i) == 1)
				return (1);
			if (reallocate_node_tokens(node, node->tokens[i], i - oi + 1))
				return (1);
		}
		if (node->tokens[i] && expand_tkn(node->tokens[i], node, ms, &i) == 1)
			return (1);
		i++;
	}
	return (0);
}
