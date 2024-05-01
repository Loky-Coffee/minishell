/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/01 21:53:40 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void expand_wildcards(t_token **token)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	char			expanded[FT_PATH_MAX] = {0};
// 	char			str_without_star[FT_PATH_MAX] = {0};
// 	int				i;
// 	int				j;

// 	i = 0;
// 	j = 0;
// 	while ((*token)->str[i] != '\0')
// 	{
// 		if ((*token)->str[i] != '*')
// 		{
// 			str_without_star[j] = (*token)->str[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	dir = opendir(".");
// 	if (dir != NULL)
// 	{
// 		while ((entry = readdir(dir)) != NULL)
// 		{
// 			if (!*str_without_star)
// 				printf("no str\n");
// 			if(ft_strnstr(entry->d_name, str_without_star, ft_strlen(entry->d_name)) != 0)
// 			{
// 				ft_strlcat(expanded, " ", ft_strlen(expanded) + 2);
// 				ft_strlcat(expanded, entry->d_name, ft_strlen(expanded) + ft_strlen(entry->d_name) + 1);
// 			}
// 		}
// 		closedir(dir);
// 	}
// 	(*token)->str = ft_calloc(ft_strlen(expanded) + 1, sizeof(char));
// 	ft_strlcpy((*token)->str, expanded, ft_strlen(expanded) + 1);
// }

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

int expand_tkn(t_token *token, t_ms *ms)
{
	char	expstr[FT_PATH_MAX];
	int		i;
	int		j;
	char	quote_mode = '\0';

	i = 0;
	j = 0;
	expand_wildcard(token);
	ft_memset(expstr, 0, sizeof(expstr));
	while (token->str[i] != '\0')
	{
		if (token->str[i] == '\\')
			expand_single_char(expstr, &j, token->str, &i);
		else if (token->str[i] == '\"' || token->str[i] == '\'')
			expand_quote(&quote_mode, expstr, &j, &token->str[i]);
		else if (token->str[i] == '$' && (token->str[i + 1] != '\0' && token->str[i + 1] != '\"' && token->str[i + 1] != ' ') && quote_mode != '\'')
		{
			expand_variable(&i, token, expstr, ms);
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
		ft_strlcpy(token->str, expstr, j + 1);
	}
	if (quote_mode != '\0')
		return (ft_error("Syntax error", "Unclosed quote detected.", NULL), 1);
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
		i++;
	}
	return (0);
}
