/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:33:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/27 14:57:12 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_unset(t_ms *ms)
{
	char	*str;
	char	key[PATH_MAX];
	int 	i;

	if (ms->tokens->next == NULL || ms->tokens->next->str == NULL)
		return (1);
	str = ms->tokens->next->str;
	ft_memset(key, 0, PATH_MAX);
	ft_strlcat(key, str, PATH_MAX);
	ft_strlcat(key, "=", PATH_MAX);
	i = 0;
	while (ms->envp && ms->envp[i] && ms->tokens->next && \
	ft_strncmp(ms->envp[i], key, ft_strlen(key)) != 0)
		i++;
	if (ms->envp && ms->envp[i] != NULL)
	{
		free(ms->envp[i]);
		while(ms->envp[i + 1] != NULL)
		{
			ms->envp[i] = ms->envp[1 + i];
			i++;
		}
		return (ms->envp[i] = NULL, 0);
	}
	return (1);
}
