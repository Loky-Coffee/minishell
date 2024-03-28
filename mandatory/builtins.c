/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 20:33:21 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/28 16:12:31 by nmihaile         ###   ########.fr       */
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
int	exec_builtin(t_builtin builtin, t_cmd *cmd, t_ms *ms, int in_pipe)
{
	int		exit_code;

	exit_code = 1;
	if (builtin == BI_ECHO)
		exit_code = ft_echo(ms);
	else if (builtin == BI_CD)
		exit_code = ft_cd(cmd, ms);
	else if (builtin == BI_PWD)
		exit_code = ft_pwd();
	else if (builtin == BI_EXPORT)
		exit_code = ft_export(ms);
	else if (builtin == BI_UNSET)
		exit_code = ft_unset(ms);
	else if (builtin == BI_ENV)
		exit_code = ft_env(ms);
	else if (builtin == BI_EXIT)
		exit_code = ft_exit(cmd, ms);
	if (in_pipe)
		terminate(ms, cmd, exit_code);
	return (exit_code);
}
