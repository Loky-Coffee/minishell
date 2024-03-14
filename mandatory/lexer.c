/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 02:30:05 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/14 11:27:48 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
// add_new_token creates and appends a new token to ms's list,
// using token type and content. It allocates memory for the token
// and content, copies content, and handles allocation failures.
// It returns 0 on success or -1 on failure.
static int	add_new_token(t_ms *ms, t_tokentype type, char *content)
{
	t_token	*new_token;

	if (type == TOKEN_NEWLINE)
		return (0);
	new_token = ft_token_new();
	if (new_token == NULL)
		return (-1);
	new_token->type = type;
	new_token->content = ft_calloc(ft_strlen(content) + 1, sizeof(char));
	if (new_token->content == NULL)
		return (free(new_token), -1);
	ft_strlcpy(new_token->content, content, strlen(content) + 1);
	ft_add_token_end(&(ms->tokens), new_token);
	return (0);
}
/* ************************************************************************** */
// check_tokens identifies special tokens for lexical analysis,
// such as pipes, redirects, and logical operators. It dynamically
// allocates memory to store token content, evaluates for
// triple, double, or single character tokens, and adds
// them to the ms's token list. After processing a token,
// it advances the index *i, frees the allocated content memory,
// and returns 1 for success or 0 if no special token was found.
static int	check_for_operators(t_ms *ms, int *i, int start)
{
	char	*content;

	content = ft_calloc(4, sizeof(char));
	if (is_tripple_token(&ms->line[*i]))
	{
		ft_strlcpy(content, &ms->line[start], 3 + 1);
		add_new_token(ms, TOKEN_TLESS, content);
		*i += 3;
		return (free(content), 1);
	}
	else if (is_double_token(&ms->line[*i]))
	{
		ft_strlcpy(content, &ms->line[start], 2 + 1);
		add_new_token(ms, is_double_token(&ms->line[*i]), content);
		*i += 2;
		return (free(content), 1);
	}
	else if (is_single_token(ms->line[*i]))
	{
		ft_strlcpy(content, &ms->line[start], 1 + 1);
		add_new_token(ms, is_single_token(ms->line[*i]), content);
		*i += 1;
		return (free(content), 1);
	}
	return (free(content), 0);
}
/* ************************************************************************** */
// ft_lexer tokenizes the input line in ms, converting it into shell tokens
// like commands and operators. It groups characters into words or operators,
// and for each group, calls add_new_token to append it to ms's token list.
// This breaks the input into tokens for parsing and execution.
void	ft_lexer(t_ms *ms)
{
	int		i;
	char	*content;
	int		len;

	i = 0;
	while (ms->line && ms->line[i])
	{
		if (check_for_operators(ms, &i, i))
			continue ;
		else if (ms->line[i] && !ft_isspace(ms->line[i]))
		{
			len = 0;
			while (ms->line[i + len] && !ft_isspace(ms->line[i + len]) \
			&& !is_operator(&ms->line[i + len]))
				len++;
			content = ft_calloc(len + 1, sizeof(char));
			ft_strlcpy(content, &ms->line[i], len + 1);
			add_new_token(ms, TOKEN_WORD, content);
			i += len;
			free(content);
		}
		else
			i++;
	}
}
