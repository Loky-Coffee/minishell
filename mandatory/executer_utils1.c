/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:15:47 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/09 21:15:08 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_exit_code(int status, t_ms *ms)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 9)
			write(2, "Terminated: 9\n", 14);
		if (WTERMSIG(status) == 15)
			write(2, "Terminated: 15\n", 15);
		ms->exit_code = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		ms->exit_code = WEXITSTATUS(status);
}

int	ft_strncmp_ignorecase(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (ft_tolower(*s1) == ft_tolower(*s2) && *s1 && n > 1)
	{
		s1++;
		s2++;
		n--;
	}
	return ((unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2));
}

t_builtin	is_builtin(t_token *token)
{
	if (token && ft_strncmp_ignorecase(token->str, "echo", 5) == 0)
		return (BI_ECHO);
	if (token && ft_strncmp_ignorecase(token->str, "cd", 3) == 0)
		return (BI_CD);
	if (token && ft_strncmp_ignorecase(token->str, "pwd", 4) == 0)
		return (BI_PWD);
	if (token && ft_strncmp_ignorecase(token->str, "export", 7) == 0)
		return (BI_EXPORT);
	if (token && ft_strncmp_ignorecase(token->str, "unset", 6) == 0)
		return (BI_UNSET);
	if (token && ft_strncmp_ignorecase(token->str, "env", 4) == 0)
		return (BI_ENV);
	if (token && ft_strncmp_ignorecase(token->str, "exit", 5) == 0)
		return (BI_EXIT);
	return (NO_BUILTIN);
}

void	ft_collaps_args(int i, t_cmd *cmd)
{
	while (cmd->args[i + 1])
	{
		cmd->args[i] = cmd->args[i + 1];
		i++;
	}
	cmd->args[i] = NULL;
}

int	ft_exec_permissions(char *cmd, int *exit_code)
{
	if (access(cmd, F_OK | X_OK) == 0)
		return (0);
	*exit_code = 126;
	ft_perror(cmd);
	return (1);
}
