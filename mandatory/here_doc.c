/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 19:03:23 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/09 10:41:41 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int del_quote_from_lim(char **lim)
{
	int i;
	int j;

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
void	expand_herdoc_manager()
{
	fprintf(stderr, "expandennnnn!!!!\n");
}

int ft_heredoc(int fd_write, char *lim)
{
	char	*line;
	int		expand_mode;

	expand_mode = del_quote_from_lim(&lim);
	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			close(fd_write);
			return (1);
		}
		if (ft_strncmp(line, lim, ft_strlen(lim) + 1) == 0)
		{
			free(line);
			close(fd_write);
			return (0);
		}
		if (expand_mode)
			expand_herdoc_manager();
		write(fd_write, line, ft_strlen(line));
		write(fd_write, "\n", 1);
		free(line);
	}
}
