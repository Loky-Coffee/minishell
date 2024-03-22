/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utoken.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:51:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/22 22:03:21 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
// ft_get_stokentype maps single characters to their corresponding token types,
// handling basic shell symbols like quotes,pipe,redirects,special characters.
static t_tokentype	ft_get_stokentype(char c)
{
	if (c == '\'')
		return (TOKEN_SQUOTE);
	else if (c == '"')
		return (TOKEN_DQUOTE);
	else if (c == '|')
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

/* ************************************************************************** */
// ft_get_dtokentype maps double-character strings
// to token types, covering redirections and logical operators.
// It checks against known operator strings, returning
// the corresponding token type or NO_TOKEN.
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

/* ************************************************************************** */
// is_single_token checks if a character matches any
// single-character shell token and returns its type.
// It iterates through a predefined list of token characters,
// leveraging ft_get_stokentype for the mapping.
t_tokentype	is_single_token(char c)
{
	char	*tokens;

	// not required:
	// => ;  \n&~$.
	// -&|><~*?$!\'"^.=\\:
	// I inserted the tilde into single quotes so that I could handle the path following ~/Desktop...
	// otherwise, everything was treated as one string.
	tokens = "\'\"|<>~";
	while (*tokens)
		if (c == *tokens++)
			return (ft_get_stokentype(c));
	return (NO_TOKEN);
}

/* ************************************************************************** */
// is_double_token determines if a string matches known
// double-character tokens for redirection and logic operators.
// Iterates through an array of such tokens, using ft_get_dtokentype
// for type identification upon a match.
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

/* ************************************************************************** */
// is_tripple_token checks for specific three-character sequences,
// such as '<<<', identifying them as unique tokens. It verifies
// the sequence length and characters, returning the appropriate token type.
t_tokentype	is_tripple_token(char *s)
{
	if (ft_strlen(s) >= 3)
		if (s[0] == '<' && s[1] == '<' && s[2] == '<')
			return (TOKEN_TLESS);
	return (NO_TOKEN);
}

/* ************************************************************************** */
// is_operator evaluates a string to determine if it matches
// known shell operator patterns, including single, double, and triple tokens.
// It sequentially checks for triple, double, then single character operators,
// returning the corresponding token type upon a match.
t_tokentype is_operator(char *s)
{
	if (is_tripple_token(s))
		return (is_tripple_token(s));
	else if (is_double_token(s))
		return (is_double_token(s));
	else if (is_single_token(*s))
		return (is_single_token(*s));
	return (NO_TOKEN);
}

/* ************************************************************************** */
// is_word checks if a given string qualifies as a word token,
// meaning it's not solely an operator and doesn't start with whitespace.
// It returns TOKEN_WORD if the criteria are met, otherwise NO_TOKEN.
t_tokentype is_word(char *str)
{
	if (str && !ft_isspace(str[0]) && !is_operator(str))
		return (TOKEN_WORD);
	return (NO_TOKEN);
}

t_tokentype tkn_is_redirect(t_token *token)
{

	if (token == NULL)
		return (NO_TOKEN);
	if (token->type == TOKEN_LESS || token->type == TOKEN_GREATER
		|| token->type == TOKEN_DLESS || token->type == TOKEN_DGREATER
		|| token->type == TOKEN_TLESS)
		return (token->type);
	return (NO_TOKEN);
}
