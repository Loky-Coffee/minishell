/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/21 07:22:56 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void check_leaks(void)
// {
// 	system("leaks minishell");
// }
// 	atexit(check_leaks);

void	cleanup_ms(t_ms *ms)
{
	if (ms->line)
		free(ms->line);
	ft_token_clear(&ms->tokens, del_token_content);
	ms->tokens = NULL;
	free_node(&ms->nodes);
}

int	main(int argc, char **argv, char **env)
{
	static t_ms	ms;
	int			exit_code;

	ms.ac = argc;
	ms.av = argv;
	ms.envp = env;
	ms.run = 1;

	restore_history();
	render_ninjashell();
	while (ms.run)
	{
		create_prompt(&ms);
		ms.line = readline(ms.prompt);
		// ms.line = readline(LIGHTGREEN"ninjaSHELL:~$ "RESET);
		if (ms.line)
			dump_history(&ms);
		else
			continue ;
		// if(ft_strncmp(ms.line, "\n\0" , 5) == 0)
		// check for EXIT
		if (ft_strncmp(ms.line, "exit\0", 5) == 0)
		{
			ms.run = FALSE;
			continue ;
		}
		if (ft_strncmp(ms.line, "\0", 1) == 0)
			continue ;


		// TOKENIZE IT
		ft_lexer(&ms);

		// render TOKENS
		// render_tokens(&ms);

		// PARSE IT aka Build TREE
		ft_parse2(ms.tokens, &ms.nodes);
		// ms.node = ft_parser(&ms);

		// render NODES
		// render_nodes(0, ms.nodes, 'R');

		// BUILDINS
		if (builtins(&ms))
		{
			cleanup_ms(&ms);
			continue ;
		}

		// EXECUTE IT
		ms.exit_code = WEXITSTATUS(exit_code);
		// printf(YELLOW"=> %i\n"RESET, ms.exit_code);


		// Clear and ninjaSHELL
		if (ft_strncmp(ms.line, "ninjashell\0", 5) == 0)
		{
			render_ninjashell();
			continue ;
		}
		exit_code = exec_manager(&ms);

		// FREE line && tokens
		cleanup_ms(&ms);
	}
	terminate(&ms, 0);
	return (0);
}
