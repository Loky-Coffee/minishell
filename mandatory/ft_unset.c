/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:33:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/13 18:14:02 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	feel_empty_line(t_ms *ms, int *flag, int i)
{
	free(ms->envp[i]);
	while (ms->envp[i + 1] != NULL)
	{
		ms->envp[i] = ms->envp[1 + i];
		i++;
		*flag = 1;
	}
	return (ms->envp[i] = NULL, 0);
}

int	ft_unset(t_ms *ms)
{
	char	*str;
	char	key[FT_PATH_MAX];
	int		i;
	int		flag;

	flag = 0;
	if (ms->tokens->next == NULL || ms->tokens->next->str == NULL)
		return (0);
	str = ms->tokens->next->str;
	ft_memset(key, 0, FT_PATH_MAX);
	ft_strlcat(key, str, FT_PATH_MAX);
	ft_strlcat(key, "=", FT_PATH_MAX);
	i = 0;
	while (ms->envp && ms->envp[i] && ms->tokens->next && \
	ft_strncmp(ms->envp[i], key, ft_strlen(key)) != 0)
		i++;
	if (ms->envp && ms->envp[i] != NULL)
		return (feel_empty_line(ms, &flag, i));
	if (flag == 1)
		return (1);
	return (0);
}
