/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/25 16:43:33 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	**create_cmd_args(t_node *node)
{
	int		i;
	int		j;
	char	**args;

	i = 0;
	while (node->tokens[i])
		i++;
	args = (char **)ft_calloc((i + 1), sizeof(char *));
	if (args == NULL)
		return (NULL);
	j = 0;
	while (j < i)
	{
		args[j] = node->tokens[j]->str;
		j++;
	}
	return (args);
}

static int	ft_exec_permissions(char *cmd, int *exit_code)
{
	if (access(cmd, X_OK) == 0)
		return (0);
	*exit_code = 126;
	return (1);
}

static int	ft_cmd_is_dir(char *cmd, int *exit_code)
{
	struct stat	file_stat;

	if (stat(cmd, &file_stat) != 0)
		return (0);
	if (S_ISDIR(file_stat.st_mode) != 0)
	{
		errno = EISDIR;
		*exit_code = 126;
		return (1);
	}
	return (0);
}

int	execute_cmd(int fdr, int fdw, t_node *node, t_ms *ms, int exit_code)
{
	int		pid;
	t_cmd	cmd;

	if (!is_word(node->tokens[0]->str))
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		if (fdr != STDIN_FILENO)
		{
			dup2(fdr, STDIN_FILENO);
			close(fdr);
		}
		if (fdw != STDOUT_FILENO)
		{
			dup2(fdw, STDOUT_FILENO);
			close(fdw);
		}
		cmd.args = create_cmd_args(node);
		if (cmd.args == NULL)
			cmd.cmdpth = ft_strdup("");
		else
			cmd.cmdpth = ft_strdup(node->tokens[0]->str);
		cmd.path = getenv("PATH");
		if (cmd.cmdpth[0] == '\0' || ft_cmd_is_dir(cmd.cmdpth, &exit_code)
			|| ft_prepend_path(&cmd.cmdpth, cmd.path)
			|| ft_exec_permissions(cmd.cmdpth, &exit_code))
		{
			ft_cmd_error(NINJASHELL, cmd.args[0], exit_code);
			ft_close_fd(fdr, fdw);
			free(cmd.cmdpth);
			free_av(cmd.args);
			terminate(ms, exit_code);
		}
		execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_close_fd(fdr, fdw);
		free(cmd.cmdpth);
		free_av(cmd.args);
		ft_perror(cmd.args[0]);
		ft_perror(NINJASHELL);
		terminate(ms, 1);
	}
	return (pid);
}

int	execute(int fdr, int fdw, t_node *node, t_ms *ms, int is_rgt)
{
	int	fdp[2];
	int	pid;
	int	status;
	int	exit_code;

	if (node == NULL)
		return (-1);
	exit_code = 1;
	status = 0;
	if (node->tokens[0] && is_operator(node->tokens[0]->str) == TOKEN_PIPE)
	{
		if (pipe(fdp))
			perror(NINJASHELL);
		if (node->left)
			status = execute(fdr, fdp[1], node->left, ms, 0);
		if (fdp[1] != STDOUT_FILENO)
			close(fdp[1]);
		if (node->right)
			status = execute(fdp[0], fdw, node->right, ms, 1);
		ft_close_fd(fdr, fdw);
		ft_close_fd(fdp[0], 0);
	}
	else if (tkn_is_redirect(node->tokens[0]) && node->tokens[1])
	{
		if (node->tokens[0]->type == TOKEN_LESS)
		{
			int	fdr_new;
			fdr_new = open(node->tokens[1]->str, O_RDONLY | O_CREAT, 0644);
			if (fdr_new != -1)
			{
				dup2(fdr_new, STDIN_FILENO);
				close(fdr_new);
				close (fdr);
			}
		}
		if (node->left)
			status = execute(fdr, fdp[1], node->left, ms, 0);
		if (fdp[1] != STDOUT_FILENO)
			close(fdp[1]);
		if (node->right)
			status = execute(fdp[0], fdw, node->right, ms, 1);
		ft_close_fd(fdr, fdw);
		ft_close_fd(fdp[0], 0);
	}
	else
	{
		if (is_rgt && node->right == NULL)
			exit_code = 127;
		pid = execute_cmd(fdr, fdw, node, ms, exit_code);
		ft_close_fd(fdr, fdw);
		if (exit_code == 127)
		{
			waitpid(pid, &status, 0);
			while (waitpid(-1, NULL, 0) > 0)
				;
		}
	}
	return (status);
}

static void save_stdfds(int *fds)
{
	dup2(STDIN_FILENO, fds[0]);
	dup2(STDOUT_FILENO, fds[1]);
}

static void set_stdfds(int *fds)
{
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
}

int	exec_manager(t_ms *ms)
{
	int	pid;
	int	status;
	int	std_fds[2];
	// struct stat stdin;
	// struct stat stdout;

	// fstat(STDIN_FILENO, &stdin);
	// fstat(STDIN_FILENO, &stdin);
	save_stdfds(std_fds);
	if (ms->nodes == NULL)
		return (-1);
	if (ms->nodes->left == NULL && ms->nodes->right == NULL && (ms->nodes->tokens && is_word(ms->nodes->tokens[0]->str)))
	{
		pid = execute_cmd(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms, 127);
		waitpid(pid, &status, 0);
	}
	else
		status = execute(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms, 0);
	return (status);
	set_stdfds(std_fds);
}
