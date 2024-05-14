/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:42:38 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:42:40 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static char	*set_path(t_ms *ms)
{
	static char	pn[FT_PATH_MAX];
	char		*slash;
	char		*path;

	path = NULL;
	if (ft_get_env_value(ms, pn, "HOME") == 0)
	{
		pn[ft_strlen(pn)] = '/';
		path = ft_strdup(pn);
	}
	else
	{
		ft_get_env_value(ms, pn, "_");
		slash = ft_strrchr(pn, '/');
		if (slash == NULL)
		{
			ft_memmove(pn, ms->av[0], ft_strlen(ms->av[0]) + 1);
			slash = ft_strrchr(pn, '/');
		}
		if (slash > pn + 2 && *(slash - 1) == '.' && *(slash - 2) == '/')
			slash -= 2;
		path = ft_substr(pn, 0, ft_strlen(pn) - ft_strlen(slash + 1));
	}
	return (path);
}

static char	*ft_historyfile(t_ms *ms)
{
	static char	historyfile[FT_PATH_MAX];
	char		*path;

	if (historyfile[0] == '\0')
	{
		path = set_path(ms);
		if (path == NULL)
			return (ft_perror("init history filepath failed"), NULL);
		ft_strlcat(historyfile, path, FT_PATH_MAX);
		ft_strlcat(historyfile, HISTORY_FILE, FT_PATH_MAX);
		free(path);
	}
	return (historyfile);
}

int	dump_history(t_ms *ms)
{
	int	fd;

	if (ms->line == NULL || ms->line[0] == '\0')
		return (0);
	add_history(ms->line);
	fd = open(ft_historyfile(ms), O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (fd == -1)
		return (1);
	if (ft_strncmp(ms->line, "", 1) == 0)
		return (close(fd), 1);
	write(fd, ms->line, ft_strlen(ms->line));
	write(fd, "\n", 1);
	close(fd);
	return (0);
}

int	restore_history(t_ms *ms)
{
	int		fd;
	char	*line;

	fd = open(ft_historyfile(ms), O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
		return (ft_perror("Loading history file failed"), 1);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		line[ft_strlen(line) - 1] = '\0';
		if (ft_strncmp(line, "", 1) != 0)
			add_history(line);
		if (line)
			free(line);
	}
	close(fd);
	return (0);
}
