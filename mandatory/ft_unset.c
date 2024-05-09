/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:33:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 11:40:38 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_valid_identifier(char *key)
{
	int	i;

	if (key[0] == '_' || ft_isalpha(key[0]))
	{
		i = 1;
		while (key[i])
		{
			if (ft_isalnum(key[i]) || key[i] == '_')
				i++;
			else
				return (1);
		}
	}
	else
		return (1);
	return (0);
}

static int	free_empty_line(t_ms *ms, int i)
{
	free(ms->envp[i]);
	while (ms->envp[i + 1] != NULL)
	{
		ms->envp[i] = ms->envp[i + 1];
		i++;
	}
	ms->envp[i] = NULL;
	return (0);
}

int	check_valid_identifier(char *str)
{
	if (is_valid_identifier(str) > 0)
	{
		if (str[0] == '-')
			return (ft_syntax_error("unset: ", &str[0], "invalid option"), 2);
		else if (ft_strchr(str, ';') != NULL)
			return (ft_syntax_error("unset: ", str, "command not found"), 127);
		return (ft_syntax_error("unset: ", str, "not a valid identifier"), 1);
	}
	return (0);
}

int	ft_unset(t_ms *ms)
{
	char	key[FT_PATH_MAX];
	int		i;
	t_token	*token;

	token = ms->tokens->next;
	if (ms->tokens->next == NULL || ms->tokens->next->str == NULL)
		return (0);
	while (token)
	{
		i = check_valid_identifier(token->str);
		if (i != 0)
			return (i);
		ft_memset(key, 0, FT_PATH_MAX);
		ft_strlcat(key, token->str, FT_PATH_MAX);
		ft_remove_unset_envvar(key, ms);
		ft_strlcat(key, "=", FT_PATH_MAX);
		i = 0;
		while (ms->envp && ms->envp[i] && token && \
		ft_strncmp(ms->envp[i], key, ft_strlen(key)) != 0)
			i++;
		if (ms->envp && ms->envp[i] != NULL)
			free_empty_line(ms, i);
		token = token->next;
	}
	return (0);
}
