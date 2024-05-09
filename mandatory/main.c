/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/09 18:15:18 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void check_leaks(void)
// {
// 	system("leaks minishell");
// }
// atexit(check_leaks);

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
	ms->default_stdin = dup(STDIN_FILENO);
	ms->default_stdout = dup(STDOUT_FILENO);
	sigint_parent_handler(0, NULL, ms);
	set_echoctl(0);
	set_signal_sigaction(SIGINT, sigint_parent_handler);
	set_signal_handler(SIGQUIT, SIG_IGN);
}

void	init(int argc, char **argv, char **env, t_ms *ms)
{
	init_ms(argc, argv, ms);
	load_env(ms, env);
	handle_arg_file(ms);
	render_ninjashell();
	restore_history(ms);
}

int	parse_it(t_ms *ms)
{
	ft_parse(ms->tokens, &ms->nodes, ms);
	if (ms->parse_error == 0)
		check_for_parse_errors(ms->nodes, ms);
	if (ms->parse_error && ms->parse_errtkn)
	{
		ms->exit_code = 258;
		ft_syntax_error("syntax error near unexpected token", \
		ms->parse_errtkn->str, NULL);
		cleanup_ms(ms);
		return (-1);
	}
	return (0);
}

void	main_loop(t_ms *ms)
{
	while (ms->run)
	{
		create_prompt(ms);
		ms->line = readline(ms->prompt);
		if (ms->line)
			dump_history(ms);
		else
		{
			ms->run = 0;
			printf("exit\n");
			continue ;
		}
		if (ft_strncmp(ms->line, "\0", 1) == 0)
			continue ;
		if (ft_lexer(ms))
		{
			cleanup_ms(ms);
			continue ;
		}
		if (parse_it(ms) == -1)
			continue ;
		exec_manager(ms);
		cleanup_ms(ms);
	}
}

int	main(int argc, char **argv, char **env)
{
	static t_ms	ms;

	init(argc, argv, env, &ms);
	main_loop(&ms);
	terminate(&ms, NULL, ms.shell_exit_code);
	return (0);
}
		// render_tokens(&ms);
		// render_nodes(0, ms.nodes, 'R');
