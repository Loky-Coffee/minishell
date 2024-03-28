/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/28 15:52:51 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int search_path(char **str)
{
	int i;

	i = 0;
	if (str && str[0])
	{
		while (str[0][i] != '=' && str[0][i] != '\0')
			i++;
	}
	return (i);
}

char	*replace_dollar(int len, t_token *token, int pos)
{
	printf("len: %d\n", len);
	printf("token->str: %s\n", token->str);
	printf("pos: %d\n", pos);
	return (NULL);
}

int expand_tkn(t_token *token)
{
	char	str[FT_PATH_MAX];
	int		i;
	int		j;
	char	quote_mode = '\0';

	i = 0;
	j = 0;
	ft_memset(str, 0, sizeof(str));
	while (token && token->str[i] != '\0')
	{
		if (token->str[i] == '\"' || token->str[i] == '\'')
		{
			if (quote_mode == '\0')
				quote_mode = token->str[i];
			else if (quote_mode == token->str[i])
				quote_mode = '\0';
			else
				str[j++] = token->str[i];
		}
		else if (token->str[i] == '$' && quote_mode != '\'')
			replace_dollar(search_path(&token->str), token, i);
		else
			str[j++] = token->str[i];
		i++;
	}
	if (j != i)
	{
		free(token->str);
		token->str = ft_calloc((j + 1), sizeof(char));
		if (token->str == NULL)
			return (1);
		ft_strlcpy(token->str, str, j + 1);
	}
	if (quote_mode != '\0')
		return (ft_error("Syntax error: Unclosed quote detected.", NULL, NULL), 1);
	return (0);
}

int	expand_node(t_node *node)
{
	t_token	*token;

	if (node == NULL || node->tokens == NULL)
		return (1);
	token = node->tokens[0];
	while (token)
	{
		if (expand_tkn(token))
			return (1);
		token = token->next;
	}
	return (0);
}
