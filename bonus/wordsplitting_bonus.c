/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplitting_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:44:43 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:44:45 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	word_splitting2(t_token	*curr, t_token *head, t_token *prev)
{
	head = prev;
	if (head)
		head->next = curr;
}

int	word_splitting(t_token *prev, t_token *token, t_token *next)
{
	size_t	i;
	size_t	len;
	t_token	*curr;
	t_token	*head;

	head = NULL;
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
		if (prev)
			prev->next = curr;
		if (head == NULL)
			word_splitting2(curr, head, prev);
		prev = curr;
		i += len;
	}
	curr->next = next;
	return (0);
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
