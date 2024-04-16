/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_old.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/16 15:30:52 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// void	check_fds(void)
// {
// 	int		fd;
// 	int		open_fd_count = 0;

// 	int prev_errno = errno;
// 	fd = 3;
// 	while (fd < OPEN_MAX)
// 	{
// 		if (fcntl(fd, F_GETFD) != -1) // TODO: DEBUG: unallowed function for debugging and finding leaks (fcntl)
// 		{
// 			// close(fd);
// 			fprintf(stderr, "%d is open(fd):\n", fd);
// 			open_fd_count++;
// 		}
// 		fd++;
// 	}
// 	errno = prev_errno;
// 	//if (LEAK_CHECK)// if some how dosnt work
// 	if (open_fd_count)
// 	{
// 		fprintf(stderr, "open fds: %d\n", open_fd_count);
// 	}
// }


// static char	**create_cmd_args(t_node *node)
// {
// 	int		i;
// 	int		j;
// 	char	**args;

// 	i = 0;
// 	while (node->tokens[i])
// 		i++;
// 	args = (char **)ft_calloc((i + 1), sizeof(char *));
// 	if (args == NULL)
// 		return (NULL);
// 	j = 0;
// 	while (j < i)
// 	{
// 		args[j] = node->tokens[j]->str;
// 		j++;
// 	}
// 	return (args);
// }

// static int	ft_exec_permissions(char *cmd, int *exit_code)
// {
// 	if (access(cmd, X_OK) == 0)
// 		return (0);
// 	*exit_code = 126;
// 	return (1);
// }

// static int	ft_cmd_is_dir(char *cmd, int *exit_code)
// {
// 	struct stat	file_stat;

// 	if (ft_strchr(cmd, '/') == NULL)
// 		return (0);
// 	if (stat(cmd, &file_stat) != 0)
// 		return (0);
// 	if (S_ISDIR(file_stat.st_mode) != 0)
// 	{
// 		errno = EISDIR;
// 		*exit_code = 126;
// 		return (1);
// 	}
// 	return (0);
// }

// static void	ft_check_cmd_is_dot(int fdr, int fdw, t_cmd *cmd, t_ms *ms)
// {
// 	if (ft_strncmp(cmd->cmdpth, ".", 2) == 0)
// 	{
// 		ft_close_fd(fdr, fdw);
// 		ft_error(cmd->cmdpth, "filename argument required", NULL);
// 		ft_putstr_fd(LIGHTRED, 2);
// 		ft_putstr_fd(cmd->cmdpth, 2);
// 		ft_putstr_fd(": usage: ", 2);
// 		ft_putstr_fd(". filename [arguments]\n"RESET, 2);
// 		terminate(ms, cmd, 2);
// 	}
// }

// static int	ft_strncmp_ignorecase(const char *s1, const char *s2, size_t n)
// {
// 	if (n == 0)
// 		return (0);
// 	while (ft_tolower(*s1) == ft_tolower(*s2) && *s1 && n > 1)
// 	{
// 		s1++;
// 		s2++;
// 		n--;
// 	}
// 	return ((unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2));
// }

// static t_builtin	is_builtin(t_token *token)
// {
// 	if (ft_strncmp_ignorecase(token->str, "echo", 5) == 0)
// 		return (BI_ECHO);
// 	if (ft_strncmp_ignorecase(token->str, "cd", 3) == 0)
// 		return (BI_CD);
// 	if (ft_strncmp_ignorecase(token->str, "pwd", 4) == 0)
// 		return (BI_PWD);
// 	if (ft_strncmp_ignorecase(token->str, "export", 7) == 0)
// 		return (BI_EXPORT);
// 	if (ft_strncmp_ignorecase(token->str, "unset", 6) == 0)
// 		return (BI_UNSET);
// 	if (ft_strncmp_ignorecase(token->str, "env", 4) == 0)
// 		return (BI_ENV);
// 	if (ft_strncmp_ignorecase(token->str, "exit", 5) == 0)
// 		return (BI_EXIT);
// 	return (NO_BUILTIN);
// }

// static int	create_cmd(t_cmd *cmd, t_node *node)
// {
// 	cmd->tokens = node->tokens;
// 	cmd->args = create_cmd_args(node);
// 	if (cmd->args == NULL)
// 		cmd->cmdpth = ft_strdup("");
// 	else
// 		cmd->cmdpth = ft_strdup(node->tokens[0]->str);
// 	return (0);
// }

int	execute_cmd(int fdr, int fdw, t_node *node, t_ms *ms, int exit_code)
{
	int			pid;
	t_cmd		cmd;
	t_builtin	builtin;

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
		create_cmd(&cmd, node);
		builtin = is_builtin(node->tokens[0]);
		if (builtin)
		{
			if (builtin != BI_EXPORT)
				if (expand_node(ms->nodes, ms))
					return (1);
			exit_code = exec_builtin(builtin, &cmd, ms, 0);
			terminate(ms, &cmd, exit_code);
		}


		ft_check_cmd_is_dot(fdr, fdw, &cmd, ms);
		ft_get_env_value(ms, cmd.path, "PATH");
		if (cmd.cmdpth[0] != '/' && (cmd.cmdpth[0] == '\0'
			|| ft_strncmp(cmd.cmdpth, "..", 3) == 0
			|| ft_cmd_is_dir(cmd.cmdpth, &exit_code)
			|| ft_prepend_path(&cmd.cmdpth, cmd.path)
			|| ft_exec_permissions(cmd.cmdpth, &exit_code)))
		{

			ft_cmd_error(NINJASHELL, cmd.args[0], exit_code);
			ft_close_fd(fdr, fdw);
			terminate(ms, &cmd, exit_code);
		}
		execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_perror(NINJASHELL);
		ft_perror(cmd.args[0]);
		ft_close_fd(fdr, fdw);
		terminate(ms, &cmd, 1);
	}
	return (pid);
}

int	execute_heredoc(int old_fdw, int *fdp, char *lim, t_ms *ms)
{
	int	pid;
	int	status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		ft_close_fd(old_fdw, 0);
		status = ft_heredoc(fdp[1], lim);
		ft_close_fd(fdp[0], 0);
		terminate(ms, NULL, status);
	}
	waitpid(pid, &status, 0);
	return (status);
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
	// if (node_is_pipe(node) == TOKEN_PIPE)
	// {
	// 	if (pipe(fdp))
	// 		perror(NINJASHELL);
	// 	if (node->left)
	// 		status = execute(fdr, fdp[1], node->left, ms, 0);
	// 	if (fdp[1] != STDOUT_FILENO)
	// 		close(fdp[1]);
	// 	if (node->right)
	// 		status = execute(fdp[0], fdw, node->right, ms, 1);
	// 	ft_close_fd(fdr, fdw);
	// 	ft_close_fd(fdp[0], 0);
	// }
	else if (node->type == NODE_REDIRECT)
	{
		if (node->tokens[0]->type == TOKEN_DLESS)
		{
			// HERE_DOC
			if (pipe(fdp))
				perror(NINJASHELL);
			ft_close_fd(fdr, 0);
			status = execute_heredoc(fdw, fdp, node->tokens[1]->str, ms);
			ft_close_fd(0, fdp[1]);
			if (node_is_word(node->left))
			{
				pid = execute_cmd(fdp[0], fdw, node->left, ms, 127);
				ft_close_fd(fdp[0], fdw);
				waitpid(pid, &status, 0);
				return (status);
			}
			fdr = fdp[0];
		}
		else if (node->tokens[0]->type == TOKEN_LESS)
		{
			// NEW STD_IN
			// fprintf(stderr, "--------------------->(%s)\n", node->tokens[1]->str);
			if (expand_node(node, ms))
				return (1);
			// fprintf(stderr, "--------------------->(%s)\n", node->tokens[1]->str);
			fdp[0] = open(node->tokens[1]->str, O_RDONLY | O_CREAT, 0644);
			if (fdp[0] != -1)
			{
				dup2(fdp[0], STDIN_FILENO);
				ft_close_fd(fdr, 0);
				fdr = fdp[0];
				ft_close_fd(fdp[0], 0);
			}
			else
				return (1);
		}
		else if (node->tokens[0]->type == TOKEN_GREATER || node->tokens[0]->type == TOKEN_DGREATER)
		{
			// NEW STD_OUT
			if (node->tokens[0]->type == TOKEN_DGREATER)
				fdp[1] = open(node->tokens[1]->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else
				fdp[1] = open(node->tokens[1]->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fdp[1] != -1)
			{
				dup2(fdp[1], STDOUT_FILENO);
				ft_close_fd(fdw, 0);
				fdw = fdp[1];
				ft_close_fd(fdp[1], 0);
			}
		}
		// close(3);
		// close(4);
		// close(5);
		// close(6);
		// check_fds();
		// close(fdw);
/////////////////////////////////////
// We have to fork the redirections
/////////////////////////////////////

		if (node->left)
			status = execute(fdr, fdw, node->left, ms, is_rgt);
		// usleep(1000);
	}
	else
	{
		if (expand_node(node, ms))
			return (1);
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

// static void save_stdfds(int *fds)
// {
// 	fds[0] = dup(STDIN_FILENO);
// 	fds[1] = dup(STDOUT_FILENO);
// }

// static void reset_stdfds(int *fds)
// {
// 	dup2(fds[0], STDIN_FILENO);
// 	dup2(fds[1], STDOUT_FILENO);
// 	close(fds[0]);
// 	close(fds[1]);
// }

// int	exec_manager(t_ms *ms)
// {
// 	int			pid;
// 	int			status;
// 	t_cmd		cmd;
// 	t_builtin	builtin;
// 	int			std_fds[2];

// 	status = 0;
// 	save_stdfds(std_fds);
// 	if (ms->nodes == NULL)
// 		return (-1);
// 	if (ms->nodes->type == NODE_COMMAND \
// 	&& ms->nodes->left == NULL && ms->nodes->right == NULL)
// 	{
// 		builtin = is_builtin(ms->nodes->tokens[0]);
// 		if (builtin)
// 		{
// 			if (builtin != BI_EXPORT)
// 				if (expand_node(ms->nodes, ms))
// 					return (1);
// 			create_cmd(&cmd, ms->nodes);
// 			ms->exit_code = exec_builtin(builtin, &cmd, ms, 0);
// 			free_cmd(&cmd);
// 			return (status);
// 		}
// 		if (expand_node(ms->nodes, ms))
// 			return (1);
// 		pid = execute_cmd(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms, 127);
// 		waitpid(pid, &status, 0);
// 		ms->exit_code = WEXITSTATUS(status);
// 	}
// 	else
// 	{
// 		status = execute(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms, 0);
// 		ms->exit_code = WEXITSTATUS(status);
// 	}
// 	reset_stdfds(std_fds);
// 	return (status);
// }
