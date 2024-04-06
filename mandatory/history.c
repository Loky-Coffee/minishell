/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:44:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/06 16:39:13 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_history_with_path(t_ms *ms)
{
	static char	str[FT_PATH_MAX];

	ft_bzero(str, FT_PATH_MAX);
	ft_strlcat(str, ms->historypath, FT_PATH_MAX);
	ft_strlcat(str, "/", FT_PATH_MAX);
	ft_strlcat(str, HISTORY_FILE, FT_PATH_MAX);
	return (&str[0]);
}

int	dump_history(t_ms *ms)
{
	int	fd;

	add_history(ms->line);
	fd = open(ft_history_with_path(ms), O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (ft_error("Can't write to history file.", NULL, NULL), 1);
	if (ft_strncmp(ms->line, "", 1) == 0)
		return (1);
	write(fd, ms->line, ft_strlen(ms->line));
	write(fd, "\n", 1);
	close(fd);
	return (0);
}

int	restore_history(t_ms *ms)
{
	int		fd;
	char	*line;

	fd = open(ft_history_with_path(ms), O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
		return (ft_error("Can't load and initalize history file.", \
		NULL, NULL), 1);
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
