/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/03 15:17:48 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	set_key(char *str, char *dst, int *pos)
{
	int	i;

	i = 0;
	ft_memset(dst, 0, FT_PATH_MAX);
	if (!ft_isalpha(str[i]))
		return (0);
	while (ft_isalnum(str[i]))
	{
		dst[i] = str[i];
		i++;
		(*pos)++;
	}
	(*pos)--;
	return (i);
}

static int	expand_variable(int *pos, t_token *token, char *str, t_ms *ms)
{
	char	key[FT_PATH_MAX];
	char	value[FT_PATH_MAX];
	char	*exit_str;

	(*pos)++;
	if (token->str[*pos] == '?')
	{
		exit_str = ft_itoa(ms->exit_code);
		ft_strlcat(str, exit_str, FT_PATH_MAX);
		return (free(exit_str), 0);
	}
	else if (token->str[*pos] == '0')
	{
		exit_str = ft_strdup(NINJASHELL);
		ft_strlcat(str, exit_str, FT_PATH_MAX);
		return (free(exit_str), 0);
	}
	set_key(&token->str[*pos], key, pos);
	ft_get_env_value(ms, value, key);
	ft_strlcat(str, value, FT_PATH_MAX);
	return (0);
}

static void	expand_single_char(char *dst, int *j, char *src, int *i)
{
	(*i)++;
	if (src[(*i)])
		dst[(*j)++] = src[(*i)];
}

static void	expand_quote(char *qm, char *dst, int *j, char *src)
{
	if (*qm == '\0')
		*qm = *src;
	else if (*qm == *src)
		*qm = '\0';
	else
		dst[(*j)++] = *src;
}

int	expand_tkn(t_token *token, t_ms *ms)
{
	char	expstr[FT_PATH_MAX];
	int		i;
	int		j;
	char	quote_mode;
	// int		do_wildcards;

	i = 0;
	j = 0;
	// do_wildcards = 0;
	expand_wildcard(token);
	quote_mode = '\0';
	ft_memset(expstr, 0, sizeof(expstr));
	while (token->str[i] != '\0')
	{
		if (token->str[i] == '\\')
			expand_single_char(expstr, &j, token->str, &i);
		else if (token->str[i] == '\"' || token->str[i] == '\'')
			expand_quote(&quote_mode, expstr, &j, &token->str[i]);
		else if (token->str[i] == '$' && (token->str[i + 1] != '\0' \
		&& token->str[i + 1] != '\"' && token->str[i + 1] != ' ') \
		&& quote_mode != '\'')
		{
			expand_variable(&i, token, expstr, ms);
			j = ft_strlen(expstr);
			if (quote_mode == '\0' && token->str[i] == '\0')
				expstr[j++] = ' ';
			// {
			// 	do_wildcards = 1;
			// }
		}
		else
			expstr[j++] = token->str[i];
		i++;
	}
// fprintf(stderr, "~~~> |%s|\n", token->str);
	// if (do_wildcards == 1)
	// 	expand_wildcard(token);
	if (j != i)
	{
		free(token->str);
		token->str = ft_calloc((j + 1), sizeof(char));
		if (token->str == NULL)
			return (1);
		ft_strlcpy(token->str, expstr, j + 1);
	}
	if (quote_mode != '\0')
		return (ft_error("Syntax error", "Unclosed quote detected.", NULL), 1);
// fprintf(stderr, "~~~> |%s|\n", token->str);
	return (0);
}

int	expand_node(t_node *node, t_ms *ms)
{
	t_token	*token;
	int		i;

	i = 0;
	if (node == NULL || node->tokens == NULL)
		return (1);
	while (node->tokens[i])
	{
		token = node->tokens[i];
		if (token && expand_tkn(token, ms) == 1)
			return (1);
		// expand_wildcard(token);
		i++;
	}
	return (0);
}
