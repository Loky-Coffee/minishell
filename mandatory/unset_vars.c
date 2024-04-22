/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:15:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/22 21:35:20 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	add_new_index_to_dptr(char ***arr)
{
	char	**new_arr;
	size_t	j;
	size_t	len;

	j = 0;
	len = 0;
	while ((*arr) && (*arr)[len])
		len++;
	// new_arr = NULL;
	new_arr = ft_calloc(len + 2, sizeof(char *));
	if (new_arr == NULL)
		return (1);
	while (j < len)
	{
		new_arr[j] = (*arr)[j];
		j++;
	}
	free(*arr);
	*arr = new_arr;
	return (0);
}

int	ft_add_unset_envvar(char *key, t_ms *ms)
{
	size_t	i;

	i = 0;
	if (add_new_index_to_dptr(&ms->unset_envvars))
		return (1);
	while (ms->unset_envvars[i])
		i++;
	ms->unset_envvars[i] = ft_strdup(key);
	return (0);
}

int	ft_remove_unset_envvar(char *key, t_ms *ms)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (ms->unset_envvars[i])
	{
		if (ft_strncmp(key, ms->unset_envvars[i], ft_strlen(key)) == 0)
		{
			free(ms->unset_envvars[i]);
			j = i + 1;
			while (ms->unset_envvars[j])
				ms->unset_envvars[i++] = ms->unset_envvars[j++];
			ms->unset_envvars[i] = 0;
			break ;
		}
		i++;
	}
	return (0);
}
