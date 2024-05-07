/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_operators.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:22:36 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/07 16:57:28 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

pid_t	exec_pipe(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	int		fd_pipe[2];
	t_node	*buff;

	if (pipe(fd_pipe))
		perror(NINJASHELL);
	buff = node->left;
	while (buff->left && buff->type != NODE_COMMAND \
	&& buff->type != NODE_SUBSHELL)
		buff = buff->left;
	buff->cfd0 = fd_pipe[0];
	if (ms->hd_interupt == 0 && node->left)
		pid = exec_intermediary(fd_in, fd_pipe[1], node->left, ms);
	ft_close_fd(fd_in, fd_pipe[1]);
	if (ms->hd_interupt == 0 && node->right)
		pid = exec_intermediary(fd_pipe[0], fd_out, node->right, ms);
	ft_close_fd(fd_pipe[0], fd_out);
	return (pid);
}

static int	exec_interm_wait(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t		pid;
	int			status;
	t_builtin	builtin;

	builtin = is_builtin(node->tokens[0]);
	if (node->parent && node->parent->type != NODE_PIPE \
	&& node->type == NODE_COMMAND && builtin)
		ms->exit_code = exec_builtin((int [2]){fd_in, fd_out}, \
		builtin, node, ms);
	else
	{
		pid = exec_intermediary(fd_in, fd_out, node, ms);
		waitpid(pid, &status, 0);
		set_exit_code(status, ms);
	}
	return (ms->exit_code);
}

int	logical_operator_manager(t_node *node, t_ms *ms, int ec)
{
	ec = 0;
	if (node->type == NODE_AND)
	{
		if (node->left)
		{
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->left, ms);
			dup2(ms->default_stdin, STDIN_FILENO);
			dup2(ms->default_stdout, STDOUT_FILENO);
		}
		if (node->right && ms->exit_code == 0)
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->right, ms);
	}
	else if (node->type == NODE_OR)
	{
		if (node->left)
		{
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->left, ms);
			dup2(ms->default_stdin, STDIN_FILENO);
			dup2(ms->default_stdout, STDOUT_FILENO);
		}
		if (node->right && ms->exit_code != 0)
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->right, ms);
	}
	return (ec);
}
