/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:57:42 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/25 04:29:38 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// char *re_alloc(char *str, int size)
// {
// 	int		i;
// 	int		x;
// 	char	*dest;


// 	i = 0;
// 	x = 0;
// 	dest = NULL;
// 	x = ft_strlen(str);
// 	dest = ft_calloc(x + size + 1, sizeof(char *));
// 	while (str[i])
// 	{
// 		dest[i] = str[i];
// 		i++;
// 	}
// 	dest[i] = 0;
// 	return (dest);
// }

int	ft_unset(t_ms *ms)
{
	printf("ICH BIN MUDE!!! GUTE NACHT!!%d\n", ms->run);
	return (0);
}

int ft_export(t_ms *ms)
{
	int i = 0;
	int j = 0;

	char **new_envp;

	while (ms->envp[i] != NULL)
		i++;
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
	char real_key[PATH_MAX];

	i = 0;
	ft_memset(real_key, 0, PATH_MAX);
	ft_strlcat(real_key, key, PATH_MAX);
	ft_strlcat(real_key, "=", PATH_MAX);
	while (ms->envp[i] != NULL && strncmp(ms->envp[i], real_key, ft_strlen(real_key)) != 0)
		i++;
	if (ms->envp[i] == NULL)
	{
		ft_memset(str, 0, PATH_MAX);
		return;
	}
	key_length = ft_strlen(real_key);
	ft_memmove(str, &ms->envp[i][key_length], ft_strlen(ms->envp[i]) - key_length + 1);
}
