/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:44:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/16 11:35:32 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_historyfile(t_ms *ms)
{
	static char	historyfile[FT_PATH_MAX];
	char		*slash;
	char		*path;

	if (historyfile[0] == '\0')
	{
		slash = ft_strrchr(ms->av[0], '/');
		path = ft_substr(ms->av[0], 0, ft_strlen(ms->av[0]) - ft_strlen(slash + 1));
		if (path == NULL)
			return (ft_perror("Initializing history filepath failed"), NULL);
		ft_strlcat(historyfile, path, FT_PATH_MAX);
		ft_strlcat(historyfile, HISTORY_FILE, FT_PATH_MAX);
		free(path);
	}
	return (historyfile);
}

int	dump_history(t_ms *ms)
{
	int	fd;

	add_history(ms->line);
	fd = open(ft_historyfile(ms), O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (ft_perror("Dump to history file failed"), 1);
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
