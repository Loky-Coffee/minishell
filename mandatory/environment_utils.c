/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 01:18:14 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 17:02:12 by aalatzas         ###   ########.fr       */
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
	if(ms->envp)
		free(ms->envp);
	ms->envp = new_envp;
	return (0);
}

void	set_shlvl(t_ms *ms)
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
