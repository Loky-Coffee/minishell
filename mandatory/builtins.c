/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:33:21 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/21 16:11:40 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//• Your shell must implement the following builtins:
//
//◦ echo with option -n ❌
//◦ cd with only a relative or absolute path ✅
//◦ pwd with no options ✅
//◦ export with no options ❌
//◦ unset with no options ❌
//◦ env with no options or arguments ✅
//◦ exit with no options ✅
int	builtins(t_ms *ms)
{
	char	current_cwd[PATH_MAX];

	if (ms->tokens == NULL)
		return (0);
	if (ft_strncmp(ms->tokens->str, "echo", 4) == 0)
		printf("echo<----\n");
	else if (ft_strncmp(ms->tokens->str, "cd\0", 3) == 0)
		ft_cd(ms);
	else if (ft_strncmp(ms->tokens->str, "pwd", 3) == 0)
		printf("%s\n", getcwd(current_cwd, sizeof(current_cwd)));
	else if (ft_strncmp(ms->tokens->str, "export", 6) == 0)
		printf("export<----\n");
	else if (ft_strncmp(ms->tokens->str, "unset", 5) == 0)
		printf("unset<----\n");
	else if (ft_strncmp(ms->tokens->str, "env", 3) == 0)
		while (*(ms->envp) != NULL)
			printf("%s\n", *ms->envp++);
	// else if (ft_strncmp(ms->tokens->str, "exit", 4) == 0)
	// 	ms->run = FALSE;
	else
		return (0);
	return (1);
}
