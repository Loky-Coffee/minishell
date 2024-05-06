/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 20:29:34 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/06 20:35:13 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	has_valid_operator(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str && str[i] == '=')
			return (0);
		else if (str && str[i] == '+' && str[i + 1] == '=')
			return (i + 2);
		i++;
	}
	return (-1);
}

int	keylen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != '=' && str[i] != '+')
		i++;
	return (i);
}

int	add_new_env_var(t_export export, t_node *node,
				t_ms *ms, t_token *token)
{
	char	**new_envp;
	int		j;

	if (token && token->str[ft_strlen(export.key)] == '+')
		ft_memmove(&token->str[ft_strlen(export.key)], \
		&token->str[ft_strlen(export.key) + 1], \
		ft_strlen(&token->str[ft_strlen(export.key) + 1]) + 1);
	j = 0;
	new_envp = NULL;
	new_envp = ft_calloc(export.i + 2, sizeof(char *));
	if (!new_envp)
		return (-1);
	while (j < export.i)
	{
		new_envp[j] = ms->envp[j];
		j++;
	}
	new_envp[export.i] = ft_strdup(token->str);
	free(ms->envp);
	ms->envp = new_envp;
	ft_remove_unset_envvar(export.key, ms);
	return (export_arguments(ms, node, token->next));
}

int	has_valid_key(int i, t_token *token, char *key)
{
	if (token == NULL)
		return (1);
	ft_memset(key, 0, FT_PATH_MAX);
	while (token->str != 0 && token->str[i] && i < 4096)
	{
		if (token->str[i] == '=' || \
	(token->str[i] == '+' && token->str[i + 1] == '='))
			break ;
		ft_strlcat(key, &token->str[i], ft_strlen(key) + 2);
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

size_t	get_envp_size(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}
