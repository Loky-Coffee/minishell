/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplitting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 09:42:58 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/07 14:11:23 by nmihaile         ###   ########.fr       */
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

static char	*ft_skipspace(char *str, size_t *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	return (&str[*i]);
}

static int	ft_get_word_len(char *str)
{
	int	len;
	
	len = 0;
	while (str[len] && !ft_isspace(str[len]))
		len++;
	return (len);
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
		ft_skipspace(token->str, &i);
		// while (token->str[i] && ft_isspace(token->str[i]))
		// 	i++;
		len = 0; // USE =========>  ft_get_word_len
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
	return (free(token->str), free(token), 0);
}

t_token	*prev_token(t_token *token, t_ms *ms)
{
	t_token	*prev;
	t_token	*buff;

	buff = ms->tokens;
	if (buff == token)
		prev = NULL;
	else
	{
		while (buff->next != token)
			buff = buff->next;		
		prev = buff;
	}
	return (prev);
}

// count defaults to 0 // and buff to NULL
int	word_split_token(t_token **token, t_ms *ms, int *count, t_token *buff)
{
	size_t	i;
	size_t	len;
	t_token	*curr;
	t_token	*next;
	t_token	*prev;
	
	if ((*token) == NULL)
		return (0);
	next = (*token)->next;
	prev = prev_token((*token), ms);
	i = 0;
	while ((*token)->str[i])
	{
		ft_skipspace((*token)->str, &i);
		len = ft_get_word_len(&(*token)->str[i]);
		curr = ft_token_new(TOKEN_WORD, ft_substr((*token)->str, i, len), NULL);
		if (curr == NULL)
			return (1);
		if (prev == NULL)
			ms->tokens = curr;
		else
			prev->next = curr;
		prev = curr;
		if (buff == NULL)
			buff = curr;
		count++;
		i += len;
	}
	curr->next = next;
	free((*token)->str);
	free((*token));
	*token = buff;
	return (0);
}
