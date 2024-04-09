/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/09 15:10:29 by nmihaile         ###   ########.fr       */
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
		free(cmd->tokens);
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
}

void	init_ms(int argc, char **argv, t_ms *ms)
{
	ms->ac = argc;
	ms->av = argv;
	ms->envp = NULL;
	ms->historypath = getenv("PWD");
	ms->run = 1;	
}

int	handle_single_arg_input(t_ms *ms)
{
	int	exit_code;

	if (ft_strncmp(ms->av[1], "-c", 3) == 0)
	{
		ms->line = ft_strdup(ms->av[2]);
		if (ft_lexer(ms))
			return (terminate(ms, NULL, 0), 0);
		ft_parse(ms->tokens, &ms->nodes);
		exit_code = exec_manager(ms);
		ms->exit_code = WEXITSTATUS(exit_code);
		cleanup_ms(ms);	
		terminate(ms, NULL, ms->shell_exit_code);
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
			ft_parse(ms->tokens, &ms->nodes);
			exit_code = exec_manager(ms);
			ms->exit_code = WEXITSTATUS(exit_code);
			cleanup_ms(ms);	
		}
		terminate(ms, NULL, ms->shell_exit_code);
	}
	else if (ms->ac == 3)
		handle_single_arg_input(ms);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	static t_ms	ms;
	int			exit_code;

	init_ms(argc, argv, &ms);
	load_env(&ms, env);
	handle_arg_file(&ms);
	restore_history(&ms);
	// render_ninjashell();
	while (ms.run)
	{
		create_prompt(&ms);
		ms.line = readline(ms.prompt);
		if (ms.line)
			dump_history(&ms);
		else
			ms.run = 0;
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
		ft_parse(ms.tokens, &ms.nodes);

		// render NODES
		// render_nodes(0, ms.nodes, 'R');

		// EXECUTE IT
		exit_code = exec_manager(&ms);
		ms.exit_code = WEXITSTATUS(exit_code);
		// printf(YELLOW"=> %i\n"RESET, ms.exit_code);

		// FREE line && tokens
		cleanup_ms(&ms);
	}
	terminate(&ms, NULL, ms.shell_exit_code);
	return (0);
}
