/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:27:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/13 18:17:21 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	has_equal_sign(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str && str[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

static int	search_env_var(t_ms *ms)
{
	int	i;

	i = 0;
	while (ms->tokens->next->str[i] != '=')
		i++;
	return (i);
}

static char	*tkn_to_str(t_token *token, t_ms *ms)
{
	expand_tkn(token, ms);
	return (ft_strdup(token->str));
}

static int	add_new_env_var(t_ms *ms, int i)
{
	char	**new_envp;
	int		j;

	j = 0;
	new_envp = NULL;
	new_envp = ft_calloc(i + 2, sizeof(char *));
	if (!new_envp)
		return (-1);
	while (j < i)
	{
		new_envp[j] = ms->envp[j];
		j++;
	}
	new_envp[i] = tkn_to_str(ms->tokens->next, ms);
	free(ms->envp);
	ms->envp = new_envp;
	return (0);
}

static int	has_valid_key(t_ms *ms, char *key)
{
	int	i;

	if (ms->tokens->next == NULL)
		return (1);
	i = 0;
	ft_memset(key, 0, FT_PATH_MAX);
	while (ms->tokens->next->str[i] && i < FT_PATH_MAX)
	{
		if (ms->tokens->next->str[i] == '=')
			break ;
		ft_strlcat(key, &ms->tokens->next->str[i], ft_strlen(key) + 2);
		i++;
	}
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

int	ft_export(t_ms *ms)
{
	int		i;
	char	key[FT_PATH_MAX];

	if (has_valid_key(ms, key) == 1)
		return (ft_error("export", ms->tokens->next->str, \
		"not a valid identifier"), 1);
	if (ms->tokens->next && has_equal_sign(ms->tokens->next->str))
		return (0);
	i = 0;
	while (ms->envp[i] != NULL && ft_strncmp(ms->envp[i], \
	ms->tokens->next->str, search_env_var(ms)) != 0)
		i++;
	if (ms->envp[i] == NULL)
		add_new_env_var(ms, i);
	else
	{
		free(ms->envp[i]);
		ms->envp[i] = tkn_to_str(ms->tokens->next, ms);
	}
	return (0);
}
