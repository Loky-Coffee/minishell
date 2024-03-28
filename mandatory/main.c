/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/28 16:34:59 by nmihaile         ###   ########.fr       */
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

// void	free_tkn_av(t_token **tokens)
// {
// 	int	i;

// 	i = 0;
// 	while (tokens[i])
// 		free(tokens[i++]);
// 	free(tokens);	
// }

void	free_cmd(t_cmd *cmd)
{
	if (cmd->tokens)
		free(cmd->tokens);
	cmd->tokens = NULL;
		// free_tkn_av(cmd->tokens);
	if (cmd->cmdpth)
		free(cmd->cmdpth);
	cmd->cmdpth = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = NULL;
}

void	cleanup_ms(t_ms *ms)
{
	free_line(ms);
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
	// We should probably get rid of the HISTORY FILE for evaluation
	ms.historypath = getenv("PWD");
	ms.run = 1;

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
		if (ft_strncmp(ms.line, "\0", 1) == 0)
			continue ;

		// TOKENIZE IT
		ft_lexer(&ms);

		// render TOKENS
		// render_tokens(&ms);

		// expande_tkn(&ms);

		// PARSE IT aka Build TREE
		ft_parse(ms.tokens, &ms.nodes);

		// render NODES
		// render_nodes(0, ms.nodes, 'R');

		// BUILDINS
		// if (builtins(&ms) && tkn_is_operator(ms.tokens) == NO_TOKEN)
		// {
		// 	cleanup_ms(&ms);
		// 	continue ;
		// }

		// EXECUTE IT
		exit_code = exec_manager(&ms);
		ms.exit_code = WEXITSTATUS(exit_code);
		printf(YELLOW"=> %i\n"RESET, ms.exit_code);

		// FREE line && tokens
		cleanup_ms(&ms);
	}
	terminate(&ms, NULL, 0);
	return (0);
}
