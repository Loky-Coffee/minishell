/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:27:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/06 18:57:56 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	search_env_var(t_ms *ms)
{
	int i;

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
	new_envp[i] = tkn_to_str(ms->tokens->next, ms);
	free(ms->envp);
	ms->envp = new_envp;
	return (0);
}

int ft_export(t_ms *ms)
{
	int i;

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