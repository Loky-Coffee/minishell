/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 19:03:23 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/03 15:08:24 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_heredoc(int fd_write, char *lim)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			close(fd_write);
			return (1);
		}
		if (ft_strncmp(line, lim, ft_strlen(lim)) == 0)
		{
			free(line);
			close(fd_write);
			return (0);
		}
		write(fd_write, line, ft_strlen(line));
		write(fd_write, "\n", 1);
		free(line);
	}
}
