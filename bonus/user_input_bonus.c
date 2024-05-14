/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user_input_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:44:12 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:44:13 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static void	sigint_additional_userinput_handler(int signal,
			siginfo_t *siginfo, void *param)
{
	(void)siginfo;
	(void)param;
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static int	line_is_unfinished(char *line)
{
	size_t	i;
	int		len;

	i = 0;
	if (line == NULL)
		return (0);
	line = ft_skipspace(line, &i);
	len = ft_strlen(line);
	if (line && line[0] != '|' && line[0] != '&' && \
	((len >= 2 && line[len - 1] == '|') || \
	(len >= 3 && line[len - 1] == '&' && line[len - 2] == '&')))
		return (1);
	return (0);
}

static void	read_additional_input(t_ms *ms, char *buf, char *new_line)
{
	while (1)
	{
		buf = readline("> ");
		if (buf)
			new_line = ft_strjoin(ms->line, buf);
		else
		{
			ft_error("syntax error", "unexpected end of file", NULL);
			if (ms->line)
				dump_history(ms);
			ms->line[0] = '\0';
			break ;
		}
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

void	read_user_input(t_ms *ms)
{
	ms->line = readline(ms->prompt);
	while (line_is_unfinished(ms->line))
	{
		set_signal_sigaction(SIGINT, sigint_additional_userinput_handler);
		read_additional_input(ms, NULL, NULL);
		set_signal_sigaction(SIGINT, sigint_parent_handler);
	}
}
