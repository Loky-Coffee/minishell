/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:42:27 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:42:29 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	del_quote_from_lim(char **lim)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if ((*lim)[0] == '\'' || (*lim)[0] == '"')
	{
		while ((*lim)[i] != '\0')
		{
			if ((*lim)[i] != '\'' && (*lim)[i] != '"')
			{
				(*lim)[j] = (*lim)[i];
				j++;
			}
			i++;
		}
		(*lim)[j] = '\0';
		return (0);
	}
	return (1);
}

int	ft_heredoc(int fd_write, char *lim, t_ms *ms)
{
	char	*line;
	int		expand_mode;
	int		err;

	err = 0;
	expand_mode = del_quote_from_lim(&lim);
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (close(fd_write), 1);
		if (ft_strncmp(line, lim, ft_strlen(lim) + 1) == 0)
		{
			free(line);
			close(fd_write);
			return (0);
		}
		if (expand_mode)
			err = expand_hd_line(&line, ms, 0, 0);
		write(fd_write, line, ft_strlen(line));
		write(fd_write, "\n", 1);
		if (err == 1)
			write(fd_write, ": bad substitution\n", 20);
		free(line);
	}
}
