/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplitting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 09:42:58 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/04 17:49:40 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	has_space(char *str)
{
	while (*str)
	{
		if (ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

int	word_splitting(t_token *prev, t_token *token, t_token *next, int *pi)
{
	size_t	i;
	size_t	len;
	t_token	*curr;
	
	if (has_space(token->str) == 0)
		return (0);
	i = 0;
	while (token->str[i])
	{
		while (token->str[i] && ft_isspace(token->str[i]))
			i++;
		len = 0;
		while (token->str[i + len] && !ft_isspace(token->str[i + len]))
			len++;
		curr = ft_token_new(TOKEN_WORD, ft_substr(token->str, i, len), NULL);
		if (curr == NULL)
			return (1);
		prev->next = curr;
		prev = curr;
		(*pi)++;
		i += len;
	}
	(*pi)--;
	curr->next = next;
	free(token->str);
	free(token);
	return (0);
}
