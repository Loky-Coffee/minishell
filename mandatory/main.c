/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/15 16:05:27 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void check_leaks(void)
{
	system("leaks minishell");
}

int	main(void)
{
	atexit(check_leaks);
	static t_ms	ms;

	ms.run = 1;
	while (ms.run)
	{
		ms.line = readline(LIGHTGREEN"minishell:~$ "RESET);
		if (ms.line)
			add_history(ms.line);
		else
			continue ;
		ft_printf("=> %s\n", ms.line);
		printf("-------------------------\n");

		// TOKENIZE IT
		ft_lexer(&ms);

		// render TOKENS
		render_tokens(&ms);

		// PARSE IT aka Build TREE
		ft_parse2(ms.tokens, &ms.nodes);
		// ms.node = ft_parser(&ms);

		// render NODES
		render_nodes(0, ms.nodes, 'R');

		// EXECUTE IT

		// check for EXIT
		if (ft_strncmp(ms.line, "exit\0", 5) == 0)
			ms.run = FALSE;

		// FREE line && tokens
		if (ms.line)
			free(ms.line);
		ft_token_clear(&ms.tokens, del_token_content);
		ms.tokens = NULL;
		free_node(&ms.nodes);
	}
	free_ms(&ms);
	return (0);
}
