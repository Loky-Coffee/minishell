/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 05:50:33 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/22 05:04:33 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void expand_wildcards(t_token **token)
{
	DIR *dir;
	struct dirent *entry;
	char expanded[FT_PATH_MAX] = {0};
	char str_without_star[FT_PATH_MAX] = {0};
	int i;
	int j;

	i = 0;
	j = 0;
	while ((*token)->str[i] != '\0')
	{
		if ((*token)->str[i] != '*')
		{
			str_without_star[j] = (*token)->str[i];
			j++;
		}
		i++;
	}
	dir = opendir(".");
	if (dir != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (!*str_without_star)
				printf("no str\n");
			if(ft_strnstr(entry->d_name, str_without_star, ft_strlen(entry->d_name)) != 0)
			{
				ft_strlcat(expanded, " ", ft_strlen(expanded) + 2);
				ft_strlcat(expanded, entry->d_name, ft_strlen(expanded) + ft_strlen(entry->d_name) + 1);
			}
		}
		closedir(dir);
	}
	(*token)->str = ft_calloc(ft_strlen(expanded) + 1, sizeof(char));
	ft_strlcpy((*token)->str, expanded, ft_strlen(expanded) + 1);
}


static int	set_key(char *str, char *dst, int *pos)
{
	int	i;

	i = 0;
	ft_memset(dst, 0, FT_PATH_MAX);
	if (!ft_isalpha(str[i]))
		return (0); // $?dfg
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
		if (exit_str)
			free(exit_str);
		return (0);
	}
	set_key(&token->str[*pos], key, pos);
	ft_get_env_value(ms, value, key);
	ft_strlcat(str, value, FT_PATH_MAX);
	return (0);
}

int expand_tkn(t_token *token, t_ms *ms)
{
	char	str[FT_PATH_MAX];
	int		i;
	int		j;
	char	quote_mode = '\0';

	i = 0;
	j = 0;
	ft_memset(str, 0, sizeof(str));
	while (token && token->str[i] != '\0')
	{
		if (token->str[i] == '\\')
		{
			i++;
			if (token->str[i])
				str[j++] = token->str[i];
		}
		else if (token->str[i] == '\"' || token->str[i] == '\'')
		{
			if (quote_mode == '\0')
				quote_mode = token->str[i];
			else if (quote_mode == token->str[i])
				quote_mode = '\0';
			else
				str[j++] = token->str[i];
		}
		else if (token->str[i] == '$' && (token->str[i + 1] != '\0' && token->str[i + 1] != '\"' && token->str[i + 1] != ' ') && quote_mode != '\'')
		{
			expand_variable(&i, token, str, ms);
			j = ft_strlen(str);
			if (quote_mode == '\0' && token->str[i] == '\0')
				str[j++] = ' ';
		}
		else if (token->str[i] == '*')// ||  (token->next && token->next->str[i] == '*')
		{
			// if (token->next && token->next->str[i] == '*')
			// 	expand_wildcards(&token->next->str[i]);
			// else
				expand_wildcards(&token);
		}
		else
			str[j++] = token->str[i];
		i++;
	}
	if (j != i)
	{
		free(token->str);
		token->str = ft_calloc((j + 1), sizeof(char));
		if (token->str == NULL)
			return (1);
		ft_strlcpy(token->str, str, j + 1);
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
	token = node->tokens[i];
	while (node->tokens[i])
	{
		if (expand_tkn(token, ms) == 1)
			return (1);
		i++;
		token = node->tokens[i];
	}
	return (0);
}
