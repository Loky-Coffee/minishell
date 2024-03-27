/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/27 15:01:26 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void check_leaks(void)
// {
// 	system("leaks minishell");
// }
// atexit(check_leaks);

void	cleanup_ms(t_ms *ms)
{
	if (ms->line)
		free(ms->line);
	ft_token_clear(&ms->tokens, del_token_content);
	ms->tokens = NULL;
	free_nodetree(&ms->nodes);
}

int	main(int argc, char **argv, char **env)
{


	static t_ms	ms;
	int			exit_code;

	ms.ac = argc;
	ms.av = argv;
	ms.envp = NULL;
	ms.historypath = getenv("PWD");
	ms.run = 1;

	// while (*(env) != NULL)
	// 	printf("%s\n", *env++);
	load_env(&ms, env);
	restore_history(&ms);
	render_ninjashell();
	while (ms.run)
	{
		create_prompt(&ms);
		ms.line = readline(ms.prompt);
		if (ms.line)
			dump_history(&ms);
		else
			continue ;
		// check for EXIT
		if (ft_strncmp(ms.line, "exit\0", 5) == 0)
		{
			ms.run = FALSE;
			continue ;
		}
		if (ft_strncmp(ms.line, "\0", 1) == 0)
			continue ;

		//  ninjaSHELL
		if (ft_strncmp(ms.line, "ninjashell\0", 5) == 0)
		{
			render_ninjashell();
			continue ;
		}

		// TOKENIZE IT
		ft_lexer(&ms);

		// render TOKENS
		render_tokens(&ms);

		expander(&ms);

		// PARSE IT aka Build TREE
		ft_parse(ms.tokens, &ms.nodes);

		// render NODES
		render_nodes(0, ms.nodes, 'R');

		// BUILDINS
		if (builtins(&ms) && tkn_is_operator(ms.tokens) == NO_TOKEN)
		{
			cleanup_ms(&ms);
			continue ;
		}

		// EXECUTE IT
		exit_code = exec_manager(&ms);
		ms.exit_code = WEXITSTATUS(exit_code);
		printf(YELLOW"=> %i\n"RESET, ms.exit_code);

		// FREE line && tokens
		cleanup_ms(&ms);
	}
	terminate(&ms, 0);
	return (0);
}
