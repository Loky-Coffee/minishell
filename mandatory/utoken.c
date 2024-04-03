/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utoken.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:51:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/03 19:41:02 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_tokentype	ft_get_stokentype(char c)
{
	if (c == '|')
		return (TOKEN_PIPE);
	else if (c == '<')
		return (TOKEN_LESS);
	else if (c == '>')
		return (TOKEN_GREATER);
	else if (c == '\n')
		return (TOKEN_NEWLINE);
	else if (c == '&')
		return (TOKEN_AND);
	else if (c == '~')
		return (TOKEN_TILDE);
	else if (c == '$')
		return (TOKEN_DOLLAR);
	return (NO_TOKEN);
}

static t_tokentype	ft_get_dtokentype(char *s)
{
	if (ft_strncmp(s, "<<", 2) == 0)
		return (TOKEN_DLESS);
	if (ft_strncmp(s, ">>", 2) == 0)
		return (TOKEN_DGREATER);
	if (ft_strncmp(s, "&&", 2) == 0)
		return (TOKEN_DAND);
	if (ft_strncmp(s, "||", 2) == 0)
		return (TOKEN_OR);
	return (NO_TOKEN);
}

t_tokentype	is_single_token(char c)
{
	char	*tokens;

	tokens = "|<>~";
	while (*tokens)
		if (c == *tokens++)
			return (ft_get_stokentype(c));
	return (NO_TOKEN);
}

t_tokentype	is_double_token(char *s)
{
	int		i;
	char	*tokens[5];

	tokens[0] = "<<";
	tokens[1] = ">>";
	tokens[2] = "&&";
	tokens[3] = "||";
	tokens[4] = NULL;
	i = 0;
	while (tokens[i])
	{
		if (ft_strncmp(tokens[i], s, 2) == 0)
			return (ft_get_dtokentype(s));
		i++;
	}
	return (NO_TOKEN);
}

t_tokentype	is_tripple_token(char *s)
{
	if (ft_strlen(s) >= 3)
		if (s[0] == '<' && s[1] == '<' && s[2] == '<')
			return (TOKEN_TLESS);
	return (NO_TOKEN);
}

t_tokentype is_operator(char *s)
{
	if (is_tripple_token(s))
		return (is_tripple_token(s));
	else if (is_double_token(s))
		return (is_double_token(s));
	else if (is_single_token(*s) == TOKEN_PIPE || is_single_token(*s) == TOKEN_LESS \
	|| is_single_token(*s) == TOKEN_GREATER)
		return (is_single_token(*s));
	return (NO_TOKEN);
}

t_tokentype is_not_word(char *s)
{
	if (is_tripple_token(s))
		return (is_tripple_token(s));
	else if (is_double_token(s))
		return (is_double_token(s));
	else if (is_single_token(*s))
		return (is_single_token(*s));
	return (NO_TOKEN);
}

t_tokentype is_word(char *str)
{
	if (str && !ft_isspace(str[0]) && !is_not_word(str))
		return (TOKEN_WORD);
	return (NO_TOKEN);
}

t_tokentype tkn_is_word(t_token *token)
{
	if (token && token->type == TOKEN_WORD)
		return (TOKEN_WORD);
	return (NO_TOKEN);
}

t_tokentype tkn_is_operator(t_token *token)
{

	if (token == NULL)
		return (NO_TOKEN);
	if (token->type == TOKEN_PIPE || token->type == TOKEN_DAND
		|| token->type == TOKEN_OR)
		return (token->type);
	return (NO_TOKEN);
}

t_tokentype tkn_is_redirect(t_token *token)
{
	if (token && (token->type == TOKEN_LESS || token->type == TOKEN_GREATER
		|| token->type == TOKEN_DLESS || token->type == TOKEN_DGREATER
		|| token->type == TOKEN_TLESS))
		return (token->type);
	return (NO_TOKEN);
}
