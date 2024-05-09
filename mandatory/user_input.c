/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 21:49:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/09 21:50:09 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	read_user_input(t_ms *ms)
{
	int		len;
	char	*buf;
	char	*new_line;

	ms->line = readline(ms->prompt);
	len = ft_strlen(ms->line);
	if (ms->line && (ms->line[len - 1] == '|' || (ms->line[len - 1] == '&' \
	&& ms->line[len - 2] == '&')))
	{
		while (1)
		{
			buf = readline("> ");
			if (buf)
				new_line = ft_strjoin(ms->line, buf);
			else
			{
				ft_error("syntax error", "unexpected end of file", NULL);
				free(ms->line);
				break ;
			}
				continue ;
			if (new_line == NULL)
			{
				free(buf);
				continue ;
			}
			free(ms->line);
			free(buf);
			ms->line = new_line;
			break ;
		}
	}
}
