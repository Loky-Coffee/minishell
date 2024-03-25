/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:33:21 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/25 04:25:58 by aalatzas         ###   ########.fr       */
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
	int		i;

	i = 0;
	if (ms->tokens == NULL)
		return (0);
	if (ft_strncmp(ms->tokens->str, "echo\0", 5) == 0)
		printf("echo<----\n");
	else if (ft_strncmp(ms->tokens->str, "cd\0", 3) == 0)
		ft_cd(ms);
	else if (ft_strncmp(ms->tokens->str, "pwd\0", 4) == 0)
		printf("%s\n", getcwd(current_cwd, sizeof(current_cwd)));
	else if (ft_strncmp(ms->tokens->str, "export\0", 7) == 0)
		ft_export(ms);
	else if (ft_strncmp(ms->tokens->str, "unset\0", 6) == 0)
		ft_unset(ms);
	else if (ft_strncmp(ms->tokens->str, "env\0", 4) == 0)
		while (ms->envp[i] != NULL)
			printf("%s\n", ms->envp[i++]);
	else
		return (0);
	return (1);
}
