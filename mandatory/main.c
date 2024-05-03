/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/03 16:51:47 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void check_leaks(void)
// {
// 	system("leaks minishell");
// }
// atexit(check_leaks);

void	free_line(t_ms *ms)
{
	if (ms->line)
	{
		free(ms->line);
		ms->line = NULL;
	}
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->tokens)
		free(cmd->tokens);	//@TODO Do we need this??
	cmd->tokens = NULL;
	if (cmd->cmdpth)
		free(cmd->cmdpth);
	cmd->cmdpth = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = NULL;
}

void	cleanup_ms(t_ms *ms)
{
	if (ms->line)
		free_line(ms);
	ms->line = NULL;
	ft_token_clear(&ms->tokens, del_token_content);
	ms->tokens = NULL;
	free_nodetree(&ms->nodes);
	ms->parse_error = 0;
	ms->parse_errtkn = NULL;
	ms->hd_interupt = 0;
}

void	init_ms(int argc, char **argv, t_ms *ms)
{
	ms->ac = argc;
	ms->av = argv;
	ms->envp = NULL;
	ms->unset_envvars = NULL;
	ms->parse_error = 0;
	ms->parse_errtkn = NULL;
	ms->hd_interupt = 0;
	ms->run = 1;
	sigint_parent_handler(0, NULL, ms);
	set_echoctl(0);
	set_signal_sigaction(SIGINT, sigint_parent_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

int	handle_single_arg_input(t_ms *ms)
{
	int	exit_code;

	if (ft_strncmp(ms->av[1], "-c", 3) == 0)
	{
		set_echoctl(1);
		set_signal_handler(SIGINT, SIG_DFL);
		set_signal_handler(SIGQUIT, SIG_DFL);
		ms->line = ft_strdup(ms->av[2]);
		if (ft_lexer(ms))
			return (terminate(ms, NULL, 0), 0);
		ft_parse(ms->tokens, &ms->nodes, ms);
		if (ms->parse_error == 0)
			check_for_parse_errors(ms->nodes, ms);
		if (ms->parse_error && ms->parse_errtkn)
		{
			ms->exit_code = 258;
			ft_error("syntax error near unexpected token", ms->parse_errtkn->str, NULL);
		}
		else
		{
			exit_code = exec_manager(ms);
			ms->exit_code = WEXITSTATUS(exit_code);
		}
		cleanup_ms(ms);
		terminate(ms, NULL, ms->exit_code);
	}
	return (ft_error("Wrong arguments", "usage", "./minishell -c 'input_line'"), terminate(ms, NULL, 1), 1);
}

int	handle_arg_file(t_ms *ms)
{
	int	fd;
	int	exit_code;

	if (ms->ac == 2)
	{
		fd = open(ms->av[1], O_RDONLY);
		if (fd == -1)
			return (ft_perror(ms->av[1]), terminate(ms, NULL, 1), 1);
		set_echoctl(1);
		set_signal_handler(SIGINT, SIG_DFL);
		set_signal_handler(SIGQUIT, SIG_DFL);
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
				ft_error("syntax error near unexpected token", ms->parse_errtkn->str, NULL);
				cleanup_ms(ms);
				continue ;
			}
			exit_code = exec_manager(ms);
			ms->exit_code = WEXITSTATUS(exit_code);
			cleanup_ms(ms);
		}
		terminate(ms, NULL, ms->exit_code);
	}
	else if (ms->ac == 3)
		handle_single_arg_input(ms);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	static t_ms	ms;

	init_ms(argc, argv, &ms);
	load_env(&ms, env);
	handle_arg_file(&ms);
	render_ninjashell();
	restore_history(&ms);
	while (ms.run)
	{
		create_prompt(&ms);
		ms.line = readline(ms.prompt);
		if (ms.line)
			dump_history(&ms);
		else
		{
			ms.run = 0;
			printf("exit\n");
			continue ;
		}

		if (ft_strncmp(ms.line, "\0", 1) == 0)
			continue ;
		// TOKENIZE IT
		if (ft_lexer(&ms))
		{
			cleanup_ms(&ms);
			continue ;
		}

		// render TOKENS
		// render_tokens(&ms);

		// PARSE IT aka Build TREE
		ft_parse(ms.tokens, &ms.nodes, &ms);
		if (ms.parse_error == 0)
			check_for_parse_errors(ms.nodes, &ms);
		if (ms.parse_error && ms.parse_errtkn)
		{
			ms.exit_code = 258;
			ft_error("syntax error near unexpected token", ms.parse_errtkn->str, NULL);
			cleanup_ms(&ms);
			continue ;
		}

		// render NODES
		// render_nodes(0, ms.nodes, 'R');

		// EXECUTE IT
		exec_manager(&ms);

		// FREE line && tokens
		cleanup_ms(&ms);
	}
	terminate(&ms, NULL, ms.shell_exit_code);
	return (0);
}
