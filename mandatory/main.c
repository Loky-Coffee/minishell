/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/20 18:15:53 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void check_leaks(void)
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **env)
{
	atexit(check_leaks);
	static t_ms	ms;

	ms.ac = argc;
	ms.av = argv;
	ms.envp = env;
	ms.run = 1;
	while (ms.run)
	{
		ms.line = readline(LIGHTGREEN"minishell:~$ "RESET);
		if (ms.line)
			add_history(ms.line);
		else
			continue ;

		// check for EXIT
		if (ft_strncmp(ms.line, "exit\0", 5) == 0)
		{
			ms.run = FALSE;
			continue ;
		}

		// TOKENIZE IT
		ft_lexer(&ms);

		// render TOKENS
		// render_tokens(&ms);

		// PARSE IT aka Build TREE
		ft_parse2(ms.tokens, &ms.nodes);
		// ms.node = ft_parser(&ms);

		// render NODES
		// render_nodes(0, ms.nodes, 'R');

		// EXECUTE IT
		exec_manager(&ms);


		// FREE line && tokens
		if (ms.line)
			free(ms.line);
		ft_token_clear(&ms.tokens, del_token_content);
		ms.tokens = NULL;
		free_node(&ms.nodes);
	}
	terminate(&ms, 0);
	return (0);
}
