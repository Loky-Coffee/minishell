/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplitting.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 09:42:58 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/10 16:10:55 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
		len = ft_get_word_len(&token->str[i]);
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

static void	set_prev_and_buff(t_token **prev, t_token *curr, \
t_token **buff, t_ms *ms)
{
	if ((*prev) == NULL)
		ms->tokens = curr;
	else
		(*prev)->next = curr;
	(*prev) = curr;
	if ((*buff) == NULL)
		*buff = curr;
}

static void	free_token_and_set(t_token **token, t_token *buff)
{
	free((*token)->str);
	free((*token));
	*token = buff;
}

// count defaults to 0 // and buff to NULL
int	word_split_token(t_token **token, t_ms *ms, int *count, t_token *buff)
{
	size_t	i;
	size_t	len;
	t_token	*curr;
	t_token	*prev;

	if ((*token) == NULL || (*token)->str == NULL || (*token)->str[0] == '\0')
		return (0);
	prev = prev_token((*token), ms);
	i = 0;
	while ((*token)->str && (*token)->str[i])
	{
		ft_skipspace((*token)->str, &i);
		len = ft_get_word_len(&(*token)->str[i]);
		curr = ft_token_new(TOKEN_WORD, ft_substr((*token)->str, i, len), NULL);
		if (curr == NULL)
			return (1);
		set_prev_and_buff(&prev, curr, &buff, ms);
		(*count)++;
		i += len;
	}
	curr->next = NULL;
	free_token_and_set(token, buff);
	return (0);
}
