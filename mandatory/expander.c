/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/04 18:01:11 by nmihaile         ###   ########.fr       */
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

int	expand_tkn(t_token *token, t_node *node, t_ms *ms)
{
	char	expstr[FT_PATH_MAX];
	int		i;
	int		j;
	char	quote_mode;
	int		do_wildcards;

	i = 0;
	j = 0;
	do_wildcards = -1;
	quote_mode = '\0';
	ft_memset(expstr, 0, sizeof(expstr));
	while (token->str[i] != '\0')
	{
		if (token->str[i] == '\\')
			expand_single_char(expstr, &j, token->str, &i);
		else if (token->str[i] == '\"' || token->str[i] == '\'')
		{
			expand_quote(&quote_mode, expstr, &j, &token->str[i]);
			do_wildcards = 0;
		}
		else if (token->str[i] == '$' && (token->str[i + 1] != '\0' \
		&& token->str[i + 1] != '\"' && token->str[i + 1] != ' ') \
		&& quote_mode != '\'')
		{
			expand_variable(&i, token, expstr, ms);
			do_wildcards = 1;
			j = ft_strlen(expstr);
			if (quote_mode == '\0' && token->str[i] == '\0')
				expstr[j++] = ' ';
		}
		else
			expstr[j++] = token->str[i];
		i++;
	}
	if (j != i)
	{
		free(token->str);
		token->str = ft_calloc((j + 1), sizeof(char));
		if (token->str == NULL)
			return (1);
	}
	ft_strlcpy(token->str, expstr, j + 1);
	if (quote_mode != '\0')
		return (ft_error("Syntax error", "Unclosed quote detected.", NULL), 1);
	if (do_wildcards == 1 && node->tokens[0]->type != TOKEN_TLESS)
		expand_wildcard(token);
	return (0);
}

static int	reallocate_node_tokens(t_node *node, int i)
{
	int		count;
	t_token	**tokens;
	t_token	*curr;

	count = i + 1;
	tokens = (t_token **)ft_calloc(count + 1, sizeof(t_token *));
	if (tokens == NULL)
		return (1);
	i = 0;
	curr = node->tokens[0];
	while (i < count)
	{
		tokens[i] = curr;
		curr = curr->next;
		i++;
	}
	free(node->tokens);
	node->tokens = tokens;
	return (0);
}

int	expand_node(t_node *node, t_ms *ms)
{
	int		i;
	int		oi;

	i = 0;
	if (node == NULL || node->tokens == NULL)
		return (1);
	while (node->tokens[i])
	{
		if (expand_wildcard(node->tokens[i]))
		{
			oi = i;
			if (word_splitting(node->tokens[i - 1], node->tokens[i], node->tokens[i]->next, &i) == 1)
				return (1);
			if (reallocate_node_tokens(node, i - oi + 1))
				return (1);
		}
		if (node->tokens[i] && expand_tkn(node->tokens[i], node, ms) == 1)
			return (1);
		i++;
	}
	return (0);
}
