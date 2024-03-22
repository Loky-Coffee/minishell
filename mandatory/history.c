/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:44:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/22 19:14:05 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*ft_history_with_path(t_ms *ms)
{
	static char	str[4096];

	ft_bzero(str, 4096);
	ft_strlcat(str, ms->historypath, 4096);
	ft_strlcat(str, "/", 4096);
	ft_strlcat(str, HISTORY_FILE, 4096);
	ft_printf("history path => %s\n", str);
	return (&str[0]);
}

int	dump_history(t_ms *ms)
{
	int	fd;
	
	add_history(ms->line);
	fd = open(ft_history_with_path(ms), O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (ft_error("Can't write to history file."), 1);
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
		return (ft_error("Can't load or initalize history file."), 1);
	while ((line = get_next_line(fd)))
	{
		line[ft_strlen(line) - 1] = '\0';
		if (ft_strncmp(line, "", 1) != 0)
			add_history(line);
		if (line)
			free(line);
	}
	close(fd);
	return (0);
}
