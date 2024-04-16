/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:32:46 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/16 23:33:51 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_prepend(const char *pre, char *s1)
{
	char	*s2;

	s2 = ft_strjoin(pre, s1);
	if (s2)
		return (s2);
	return (s1);
}

static char	*ft_append(char *s1, char *app)
{
	char	*s2;

	if (s1 == NULL)
		return (NULL);
	s2 = ft_strjoin(s1, app);
	if (s2)
	{
		free (s1);
		return (s2);
	}
	return (s1);
}

static char	**ft_get_paths(char *envpaths)
{
	size_t	i;
	char	**paths;

	i = 0;
	paths = ft_split(envpaths, ':');
	if (paths == NULL)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		paths[i] = ft_append(paths[i], "/");
		if (paths[i] == NULL)
			return (NULL);
		i++;
	}
	return (paths);
}

int	ft_prepend_path(char **cmd, char *envpaths, int *exit_code)
{
	size_t	i;
	char	**paths;
	char	*pthcmd;

	paths = ft_get_paths(envpaths);
	if (paths == NULL)
		return (ft_perror(*cmd), 1);
	i = 0;
	while (paths[i])
	{
		pthcmd = ft_prepend(paths[i], *cmd);
		if (access(pthcmd, F_OK) == 0)
		{
			free(*cmd);
			*cmd = pthcmd;
			free_av(paths);
			return (0);
		}
		free(pthcmd);
		i++;
	}
	free_av(paths);
	if (*cmd && (*cmd)[0] == '.' && access(*cmd, F_OK) == 0)
		return (0);
	*exit_code = 127;
	ft_error(*cmd, "command not found", NULL);
	return (1);
}
