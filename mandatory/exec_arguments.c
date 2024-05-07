/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_arguments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 17:45:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/07 18:08:47 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	handle_single_arg_input(t_ms *ms)
{
	if (ft_strncmp(ms->av[1], "-c", 3) == 0)
	{
		reset_signals();
		ms->line = ft_strdup(ms->av[2]);
		if (ft_lexer(ms))
			return (terminate(ms, NULL, 0), 0);
		ft_parse(ms->tokens, &ms->nodes, ms);
		if (ms->parse_error == 0)
			check_for_parse_errors(ms->nodes, ms);
		if (ms->parse_error && ms->parse_errtkn)
		{
			ms->exit_code = 258;
			ft_error("syntax error near unexpected token", \
			ms->parse_errtkn->str, NULL);
		}
		else
			exec_manager(ms);
		cleanup_ms(ms);
		terminate(ms, NULL, ms->exit_code);
	}
	return (ft_error("Wrong arguments", "usage", \
	"./minishell -c 'input_line'"), terminate(ms, NULL, 1), 1);
}

static void	exec_lines_from_file(int fd, t_ms *ms)
{
	while (1)
	{
		ms->line = get_next_line(fd);
		if (ms->line == NULL)
			break ;
		if (ft_lexer(ms))
		{
			cleanup_ms(ms);
			continue ;
		}
		ft_parse(ms->tokens, &ms->nodes, ms);
		if (ms->parse_error == 0)
			check_for_parse_errors(ms->nodes, ms);
		if (ms->parse_error && ms->parse_errtkn)
		{
			ms->exit_code = 258;
			ft_error("syntax error near unexpected token", \
			ms->parse_errtkn->str, NULL);
			cleanup_ms(ms);
			continue ;
		}
		exec_manager(ms);
		cleanup_ms(ms);
	}
}

int	handle_arg_file(t_ms *ms)
{
	int	fd;

	if (ms->ac == 2)
	{
		fd = open(ms->av[1], O_RDONLY);
		if (fd == -1)
			return (ft_perror(ms->av[1]), terminate(ms, NULL, 1), 1);
		reset_signals();
		exec_lines_from_file(fd, ms);
		terminate(ms, NULL, ms->exit_code);
	}
	else if (ms->ac >= 3)
		handle_single_arg_input(ms);
	return (0);
}
