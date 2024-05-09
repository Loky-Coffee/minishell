/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 17:41:10 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

pid_t	exec_intermediary(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t		pid;
	t_builtin	builtin;

	pid = -1;
	if (node == NULL)
		return (0);
	if (node->tokens[1] == NULL)
		expand_node(node, ms);
	builtin = is_builtin(node->tokens[0]);
	if (node->type == NODE_COMMAND && builtin)
		pid = exec_fork_builtin((int [2]){fd_in, fd_out}, builtin, node, ms);
	else if (node->type == NODE_COMMAND)
		pid = exec_cmd(fd_in, fd_out, node, ms);
	else if (node->type == NODE_SUBSHELL)
		pid = exec_subshell(fd_in, fd_out, node, ms);
	else if (node->type == NODE_REDIRECT)
		pid = redirect_manager(fd_in, fd_out, node, ms);
	else if (node->type == NODE_PIPE)
		pid = exec_pipe(fd_in, fd_out, node, ms);
	else if (node->type == NODE_AND || node->type == NODE_OR)
		pid = logical_operator_manager(node, ms, 0);
	return (pid);
}

static void	launch_cmd_or_subshell(t_ms *ms)
{
	pid_t		pid;
	int			status;

	status = 0;
	if (ms->nodes->type == NODE_COMMAND)
	{
		pid = exec_cmd(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		waitpid(pid, &status, 0);
		set_exit_code(status, ms);
	}
	else if (ms->nodes->type == NODE_SUBSHELL)
	{
		pid = exec_subshell(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		waitpid(pid, &status, 0);
		set_exit_code(status, ms);
	}
}

static void	launche_redirects(t_ms *ms)
{
	pid_t		pid;
	int			status;

	status = 0;
	pid = redirect_manager(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
	if (pid > 255)
	{
		waitpid(pid, &status, 0);
		set_exit_code(status, ms);
	}
	else
		ms->exit_code = 1;
}

static void	launche_pipeline(t_ms *ms)
{
	pid_t		pid;
	int			status;

	status = 0;
	pid = exec_pipe(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
	if (pid > 255)
	{
		waitpid(pid, &status, 0);
		while (waitpid(-1, NULL, 0) > 0)
			;
		set_exit_code(status, ms);
	}
	else
		ms->exit_code = 1;
}

int	exec_manager(t_ms *ms)
{
	t_builtin	builtin;

	if (ms->nodes == NULL)
		return (-1);
	if (ms->nodes->tokens[1] == NULL)
		expand_node(ms->nodes, ms);
	builtin = is_builtin(ms->nodes->tokens[0]);
	if (ms->nodes->type == NODE_COMMAND && builtin)
		ms->exit_code = exec_builtin((int [2]) \
		{STDIN_FILENO, STDOUT_FILENO}, builtin, ms->nodes, ms);
	else if (ms->nodes->type == NODE_COMMAND \
	|| ms->nodes->type == NODE_SUBSHELL)
		launch_cmd_or_subshell(ms);
	else if (ms->nodes->type == NODE_REDIRECT)
		launche_redirects(ms);
	else if (ms->nodes->type == NODE_PIPE)
		launche_pipeline(ms);
	else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
		logical_operator_manager(ms->nodes, ms, 0);
	dup2(ms->default_stdin, STDIN_FILENO);
	dup2(ms->default_stdout, STDOUT_FILENO);
	set_echoctl(0);
	return (ms->exit_code);
}
