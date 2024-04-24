/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:57:42 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/24 14:47:58 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_valid_envkey(char *key)
{
	int	i;

	i = 0;
	if (key[i] == '_' || ft_isalpha(key[i]))
	{
		i++;
		while (key[i])
		{
			if (ft_isalnum(key[i]) || key[i] == '_')
				i++;
			else
				return (0);
		}
	}
	else
		return (0);
	return (1);
}

int	add_new_index_to_envp(t_ms *ms, int len)
{
	char	**new_envp;
	int		j;

	j = 0;
	new_envp = NULL;
	new_envp = ft_calloc(len + 2, sizeof(char *));
	if (new_envp == NULL)
		return (-1);
	while (j < len)
	{
		new_envp[j] = ms->envp[j];
		j++;
	}
	free(ms->envp);
	ms->envp = new_envp;
	return (0);
}

static void	set_shlvl(t_ms *ms)
{
	int		lvl;
	char	lvlstr[FT_PATH_MAX];
	char	*newlvl;
	
	ft_get_env_value(ms, lvlstr, "SHLVL");
	lvl = ft_atoi(lvlstr);
	lvl++;
	newlvl = ft_itoa(lvl);
	ft_setenv("SHLVL", newlvl, ms);
	free(newlvl);
}

void	load_env(t_ms *ms, char **env)
{
	static int		i;
	static size_t	count_pointer;
	char			pwd[FT_PATH_MAX];

	while (env[count_pointer])
		count_pointer++;
	ms->envp = ft_calloc(count_pointer + 1, sizeof(char *));
	while (env[i])
	{
		ms->envp[i] = ft_strdup(env[i]);
		i++;
	}
	ms->envp[i] = NULL;
	getcwd(pwd, FT_PATH_MAX);
	ft_setenv("PWD", pwd, ms);
	set_shlvl(ms);
}

char	*ft_env_getkey(char *str)
{
	size_t	i;
	char	key[FT_PATH_MAX];

	if (str == NULL)
		return (NULL);
	i = 0;
	ft_memset(key, 0, FT_PATH_MAX);
	while (str && str[i] && str[i] != '=' && i < FT_PATH_MAX)
	{
		key[i] = str[i];
		i++;
	}
	return (ft_strdup(key));
}

char	*ft_env_getvalue(char *str)
{
	size_t	i;
	size_t	j;
	char	value[FT_PATH_MAX];

	i = 0;
	j = 0;
	if (str == NULL)
		return (NULL);
	ft_memset(value, 0, FT_PATH_MAX);
	while (str[i] && str[i] != '=' && i < FT_PATH_MAX)
		i++;
	if (str[i] == '=')
	{
		i++;
		while (str[i] && i < FT_PATH_MAX)
			value[j++] = str[i++];
		return (ft_strdup(value));
	}
	return (NULL);
}

int	ft_get_env_value(t_ms *ms, char *str, char *key)
{
	int		i;
	int		key_length;
	char	real_key[FT_PATH_MAX];

	i = 0;
	ft_memset(real_key, 0, FT_PATH_MAX);
	ft_strlcat(real_key, key, FT_PATH_MAX);
	ft_strlcat(real_key, "=", FT_PATH_MAX);
	while (ms->envp[i] != NULL \
	&& strncmp(ms->envp[i], real_key, ft_strlen(real_key)) != 0)
		i++;
	if (ms->envp[i] == NULL)
	{
		ft_memset(str, 0, FT_PATH_MAX);
		return (1);
	}
	key_length = ft_strlen(real_key);
	ft_memmove(str, &ms->envp[i][key_length], \
	ft_strlen(ms->envp[i]) - key_length + 1);
	return (0);
}

int	ft_setenv(char *key, char *value, t_ms *ms)
{
	size_t	i;
	size_t	len;
	char	*nkv;
	
	if (!is_valid_envkey(key))
		return (1);
	i = 0;
	while (ms->envp[i] != NULL && strncmp(ms->envp[i], key, ft_strlen(key)) != 0)
		i++;
	if (ms->envp[i] == NULL && add_new_index_to_envp(ms, i) == -1)
			return (1);
	len = ft_strlen(key) + ft_strlen(value) + 2;
	nkv = (char *)ft_calloc(len, sizeof(char));
	if (nkv == NULL)
		return (1);
	ft_strlcat(nkv, key, len);
	ft_strlcat(nkv, "=", len);
	ft_strlcat(nkv, value, len);
	if (ms->envp[i])
		free(ms->envp[i]);
	ms->envp[i] = nkv;
	return (0);
}
