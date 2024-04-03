/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:57:42 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/03 19:39:11 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	load_env(t_ms *ms, char **env)
{
	static int	i;
	static size_t		count_pointer;

	while(env[count_pointer])
		count_pointer++;
	ms->envp = ft_calloc(count_pointer + 1, sizeof(char *));
	while(env[i])
	{
		ms->envp[i] = ft_strdup(env[i]);
		i++;
	}
	ms->envp[i] = NULL;
}

void	ft_get_env_value(t_ms *ms, char *str, char *key)
{
	int i;
	int key_length;
	char real_key[FT_PATH_MAX];

	i = 0;
	ft_memset(real_key, 0, FT_PATH_MAX);
	ft_strlcat(real_key, key, FT_PATH_MAX);
	ft_strlcat(real_key, "=", FT_PATH_MAX);
	while (ms->envp[i] != NULL && strncmp(ms->envp[i], real_key, ft_strlen(real_key)) != 0)
		i++;
	if (ms->envp[i] == NULL)
	{
		ft_memset(str, 0, FT_PATH_MAX);
		return;
	}
	key_length = ft_strlen(real_key);
	ft_memmove(str, &ms->envp[i][key_length], ft_strlen(ms->envp[i]) - key_length + 1);
}
