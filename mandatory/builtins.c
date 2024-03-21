/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:33:21 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/21 08:56:37 by aalatzas         ###   ########.fr       */
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

	if (ft_strncmp(ms->tokens->str, "echo", 4) == 0)
		printf("echo<----\n");
	else if (ft_strncmp(ms->tokens->str, "cd", 2) == 0)
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
