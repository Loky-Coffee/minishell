/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 02:30:05 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/27 10:06:13 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
// add_new_token creates and appends a new token to ms's list,
// using token type and str. It allocates memory for the token
// and str, copies str, and handles allocation failures.
// It returns 0 on success or -1 on failure.
static int	add_new_token(t_ms *ms, t_tokentype type, char *str)
{
	t_token	*new_token;

	if (type == TOKEN_NEWLINE)
		return (0);
	new_token = ft_token_new();
	if (new_token == NULL)
		return (-1);
	new_token->type = type;
	new_token->str = str;
	new_token->next = NULL;
	ft_add_token_end(&(ms->tokens), new_token);
	return (0);
}

/* ************************************************************************** */
// check_tokens identifies special tokens for lexical analysis,
// such as pipes, redirects, and logical operators. It dynamically
// allocates memory to store token str, evaluates for
// triple, double, or single character tokens, and adds
// them to the ms's token list. After processing a token,
// it advances the index *i, frees the allocated str memory,
// and returns 1 for success or 0 if no special token was found.
static int	check_for_operators(t_ms *ms, int *i, int start)
{
	char	*str;

	str = ft_calloc(4, sizeof(char));
	if (is_tripple_token(&ms->line[*i]))
	{
		ft_strlcpy(str, &ms->line[start], 3 + 1);
		add_new_token(ms, TOKEN_TLESS, str);
		*i += 3;
		return (1);
	}
	else if (is_double_token(&ms->line[*i]))
	{
		ft_strlcpy(str, &ms->line[start], 2 + 1);
		add_new_token(ms, is_double_token(&ms->line[*i]), str);
		*i += 2;
		return (1);
	}
	else if (is_single_token(ms->line[*i]))
	{
		ft_strlcpy(str, &ms->line[start], 1 + 1);
		add_new_token(ms, is_single_token(ms->line[*i]), str);
		*i += 1;
		return (1);
	}
	return (free(str), 0);
}

/* ************************************************************************** */
// ft_lexer tokenizes the input line in ms, converting it into shell tokens
// like commands and operators. It groups characters into words or operators,
// and for each group, calls add_new_token to append it to ms's token list.
// This breaks the input into tokens for parsing and execution.
void	ft_lexer(t_ms *ms)
{
	int		i;
	char	*str;
	int		len;
	bool	stop;

	i = 0;
	stop = 0;
	while (ms->line && ms->line[i])
	{
		if (check_for_operators(ms, &i, i))
			continue ;
		else if (ms->line[i] && !ft_isspace(ms->line[i]))
		{
			len = 0;
			if (ms->line[i + len] == '\"' || ms->line[i + len] == '\'')
			{
				while(ms->line[i + len])
				{
					if (is_operator(&ms->line[i + len]) != NO_TOKEN && ms->line[i + len + 1] == ' ' && ms->line[i + len - 1] == ' ')
					{
						len--;
						break;
					}
					if (ms->line[i + len] == '\\')
						len++;
					len++;
				}
				len++;
			}
			else
			{
				while (ms->line[i + len] && !ft_isspace(ms->line[i + len]) \
				&& !is_not_word(&ms->line[i + len]))
					len++;
			}
			str = ft_calloc(len + 1, sizeof(char));
			ft_strlcpy(str, &ms->line[i], len + 1);
			add_new_token(ms, TOKEN_WORD, str);
			i += len;
		}
		else
			i++;
	}
}
