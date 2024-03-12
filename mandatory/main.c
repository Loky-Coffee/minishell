/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:44:50 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/12 21:52:32 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(void)
{
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

		// TOKENIZE IT
		ft_lexer(&ms);

		// render TOKENS
		render_tokens(&ms);

		// PARSE IT aka Build TREE
		ft_parser(&ms);

		// EXEXUTE IT

		// check for EXIT
		if (ft_strncmp(ms.line, "exit\0", 5) == 0)
			ms.run = FALSE;

		// FREE line && tokens
		if (ms.line)
			free(ms.line);
		ft_lstclear(&ms.tokens, del_token);
		ms.tokens = NULL;
	}

	free_ms(&ms);
	return (0);
}
