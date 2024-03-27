/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:27:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/27 02:51:30 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	search_path(t_ms *ms)
{
	int i;

	i = 0;
	while (ms->tokens->next->str[i] != '=')
		i++;
	return (i);
}

static int	path_not_exist(t_ms *ms, int i)
{
	char **new_envp;
	int j;

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
	new_envp[i] = ft_strdup(ms->tokens->next->str);
	ms->envp = new_envp;
	return (0);
}

int ft_export(t_ms *ms)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (ms->envp[i] != NULL && ft_strncmp(ms->envp[i], \
	 ms->tokens->next->str, search_path(ms)) != 0)
		i++;
	if (!ms->envp[i])
		path_not_exist(ms, i);
	else
	{
		free(ms->envp[i]);
		//@TODO: Expand the str first
		ms->envp[i] = ft_strdup(ms->tokens->next->str);
	}
	return (0);
}