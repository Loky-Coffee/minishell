/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_exec_cmd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:42:46 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/07 15:43:12 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_and_launch_cmd(int fd_in, int fd_out, t_cmd *cmd, t_ms *ms)
{
	int		exit_code;

	exit_code = 0;
	ft_cmd_is_empty(fd_in, fd_out, cmd, ms);
	ft_cmd_is_dot(cmd, ms);
	ft_get_env_value(ms, cmd->path, "PATH");
	if (ft_cmd_is_dir(cmd->cmdpth, &exit_code)
		|| ft_cmd_has_slash(cmd, &exit_code)
		|| ft_cmd_is_dotdot(cmd, &exit_code)
		|| ft_prepend_path(&cmd->cmdpth, cmd->path, &exit_code)
		|| ft_exec_permissions(cmd->cmdpth, &exit_code))
	{
		if (exit_code == 0)
			exit_code = 1;
		ms->exit_code = exit_code;
		ft_close_fd(fd_in, fd_out);
		terminate(ms, cmd, exit_code);
	}
	execve(cmd->cmdpth, cmd->args, ms->envp);
	ft_perror(cmd->args[0]);
	ft_close_fd(fd_in, fd_out);
	terminate(ms, cmd, 1);
}

pid_t	exec_cmd(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	t_cmd	cmd;

	pid = fork();
	if (pid == 0)
	{
		reset_signals();
		ft_close_fd(node->cfd0, node->cfd1);
		if (expand_node(node, ms, 0))
			return (-1);
		create_cmd(&cmd, node);
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		check_and_launch_cmd(fd_in, fd_out, &cmd, ms);
	}
	ft_close_fd(fd_in, fd_out);
	return (pid);
}

pid_t	exec_subshell(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	t_cmd	cmd;

	pid = fork();
	if (pid == 0)
	{
		ft_close_fd(node->cfd0, node->cfd1);
		ft_close_fd(fd_in, fd_out);
		if (expand_node(node, ms, 0))
			return (-1);
		create_subshell_cmd(&cmd, node, ms);
		reset_signals();
		execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_perror(cmd.args[0]);
		ft_close_fd(fd_in, fd_out);
		terminate(ms, &cmd, 1);
	}
	ft_close_fd(fd_in, fd_out);
	return (pid);
}

void	execute_heredoc(int *fd_in, int *fd_out, char *lim, t_ms *ms)
{
	int	pid_status[2];
	int	fd_pipe[2];

	pid_status[1] = 0;
	if (pipe(fd_pipe))
		perror(NINJASHELL);
	ft_close_fd(*fd_in, 0);
	*fd_in = dup(fd_pipe[0]);
	dup2(ms->default_stdin, STDIN_FILENO);
	pid_status[0] = fork();
	if (pid_status[0] == -1)
		perror(NINJASHELL);
	if (pid_status[0] == 0)
	{
		set_signal_handler(SIGINT, SIG_DFL);
		pid_status[1] = ft_heredoc(fd_pipe[1], lim);
		ft_close_fd(fd_pipe[0], fd_pipe[1]);
		ft_close_fd(*fd_in, *fd_out);
		terminate(ms, NULL, pid_status[1]);
	}
	ft_close_fd(fd_pipe[0], fd_pipe[1]);
	waitpid(pid_status[0], &pid_status[1], 0);
	set_exit_code(pid_status[1], ms);
	if (WIFSIGNALED(pid_status[1]))
		ms->hd_interupt = 1;
}

void	execute_herestring(int *fd_in, int *fd_out, char *str, t_ms *ms)
{
	int	pid;
	int	fd_pipe[2];

	if (pipe(fd_pipe))
		perror(NINJASHELL);
	ft_close_fd(*fd_in, 0);
	*fd_in = dup(fd_pipe[0]);
	pid = fork();
	if (pid == -1)
		perror(NINJASHELL);
	if (pid == 0)
	{
		write(fd_pipe[1], str, ft_strlen(str));
		write(fd_pipe[1], "\n", 1);
		ft_close_fd(fd_pipe[0], fd_pipe[1]);
		ft_close_fd(*fd_in, *fd_out);
		terminate(ms, NULL, 0);
	}
	ft_close_fd(fd_pipe[0], fd_pipe[1]);
}
