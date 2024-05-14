/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_builtins_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:40:27 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:40:29 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	run_builtin(int *fds, t_builtin builtin, t_cmd *cmd, t_ms *ms)
{
	int		exit_code;

	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	exit_code = 1;
	if (builtin == BI_ECHO)
		exit_code = ft_echo(cmd);
	else if (builtin == BI_CD)
		exit_code = ft_cd(cmd, ms);
	else if (builtin == BI_PWD)
		exit_code = ft_pwd();
	else if (builtin == BI_EXPORT)
		exit_code = ft_export(cmd->node, ms);
	else if (builtin == BI_UNSET)
		exit_code = ft_unset(cmd, ms);
	else if (builtin == BI_ENV)
		exit_code = ft_env(ms);
	else if (builtin == BI_EXIT)
		exit_code = ft_exit(cmd, ms);
	ft_close_fd(fds[0], fds[1]);
	return (exit_code);
}

pid_t	fork_run_builtin(int *fds, t_builtin builtin, t_cmd *cmd, t_ms *ms)
{
	pid_t	pid;
	int		exit_code;

	pid = fork();
	if (pid == 0)
	{
		exit_code = run_builtin(fds, builtin, cmd, ms);
		if (exit_code != 0)
			ms->exit_code = exit_code;
		terminate(ms, cmd, exit_code);
	}
	ft_close_fd(fds[0], fds[1]);
	return (pid);
}

int	exec_builtin(int fds[2], t_builtin builtin, t_node *node, t_ms *ms)
{
	int			exit_code;
	t_cmd		cmd;

	if (builtin != BI_EXPORT)
		if (expand_node(node, ms))
			return (-1);
	if (node)
		create_cmd(&cmd, node);
	exit_code = run_builtin((int [2]){fds[0], fds[1]}, builtin, &cmd, ms);
	free_cmd(&cmd);
	return (exit_code);
}

pid_t	exec_fork_builtin(int fds[2], t_builtin builtin, t_node *node, t_ms *ms)
{
	pid_t	pid;
	t_cmd	cmd;

	if (builtin != BI_EXPORT)
		if (expand_node(node, ms))
			return (-1);
	if (node)
		create_cmd(&cmd, node);
	pid = fork_run_builtin((int [2]){fds[0], fds[1]}, builtin, &cmd, ms);
	free_cmd(&cmd);
	return (pid);
}
