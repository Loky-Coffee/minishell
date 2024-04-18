/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/18 06:21:48 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	check_fds(void)
{
	int		fd;
	int		open_fd_count = 0;

	int prev_errno = errno;
	fd = 3;
	while (fd < OPEN_MAX)
	{
		if (fcntl(fd, F_GETFD) != -1) // TODO: DEBUG: unallowed function for debugging and finding leaks (fcntl)
		{
			// close(fd);
			fprintf(stderr, "%d is open(fd):\n", fd);
			open_fd_count++;
		}
		fd++;
	}
	errno = prev_errno;
	//if (LEAK_CHECK)// if some how dosnt work
	if (open_fd_count)
	{
		fprintf(stderr, "open fds: %d\n", open_fd_count);
	}
}

int	exec_intermediary(int fd_in, int fd_out, t_node *node, t_ms *ms);

static void save_stdfds(int *fds)
{
	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
}

static void reset_stdfds(int *fds)
{
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
}

static int	ft_strncmp_ignorecase(const char *s1, const char *s2, size_t n)
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

static t_builtin	is_builtin(t_token *token)
{
	if (ft_strncmp_ignorecase(token->str, "echo", 5) == 0)
		return (BI_ECHO);
	if (ft_strncmp_ignorecase(token->str, "cd", 3) == 0)
		return (BI_CD);
	if (ft_strncmp_ignorecase(token->str, "pwd", 4) == 0)
		return (BI_PWD);
	if (ft_strncmp_ignorecase(token->str, "export", 7) == 0)
		return (BI_EXPORT);
	if (ft_strncmp_ignorecase(token->str, "unset", 6) == 0)
		return (BI_UNSET);
	if (ft_strncmp_ignorecase(token->str, "env", 4) == 0)
		return (BI_ENV);
	if (ft_strncmp_ignorecase(token->str, "exit", 5) == 0)
		return (BI_EXIT);
	return (NO_BUILTIN);
}

static int	create_cmd(t_cmd *cmd, t_node *node)
{
	int		i;
	int		j;
	char	**args;

	cmd->tokens = node->tokens;
		i = 0;
	while (node->tokens[i])
		i++;
	args = (char **)ft_calloc((i + 1), sizeof(char *));
	if (args == NULL)
		return (0);
	j = 0;
	while (j < i)
	{
		args[j] = node->tokens[j]->str;
		j++;
	}
	cmd->args = args;
	if (cmd->args == NULL)
		cmd->cmdpth = ft_strdup("");
	else
		cmd->cmdpth = ft_strdup(node->tokens[0]->str);
	return (0);
}

static void	ft_cmd_is_dot(t_cmd *cmd, t_ms *ms)
{
	if (ft_strncmp(cmd->cmdpth, ".", 2) == 0)
	{
		ft_error(cmd->cmdpth, "filename argument required", NULL);
		ft_putstr_fd(LIGHTCYAN "\n", 2);
		ft_putstr_fd(cmd->cmdpth, 2);
		ft_putstr_fd(": usage: ", 2);
		ft_putstr_fd(". filename [arguments]"RESET, 2);
		terminate(ms, cmd, 2);
	}
}

static int	ft_cmd_is_dotdot(t_cmd *cmd, int *exit_code)
{
	if (ft_strncmp(cmd->cmdpth, "..", 3) == 0)
	{
		ft_error(cmd->cmdpth, "command not found", NULL);
		*exit_code = 127;
		return (1);
	}
	return (0);
}

static int	ft_cmd_has_slash(t_cmd *cmd, int *exit_code)
{
	if (cmd->cmdpth[0] == '/')
	{
		*exit_code = 127;
		return (ft_error(cmd->cmdpth, "No such file or directory", NULL), 1);
	}
	return (0);
}

static int	ft_cmd_is_dir(char *cmd, int *exit_code)
{
	struct stat	file_stat;

	if (ft_strchr(cmd, '/') == NULL || ft_strchr(cmd, '.') == NULL)
		return (0);
	if (stat(cmd, &file_stat) != 0)
		return (0);
	if (S_ISDIR(file_stat.st_mode) != 0)
	{
		*exit_code = 126;
		ft_error(cmd, "is a directory", NULL);
		return (1);
	}
	return (0);
}

static int	ft_exec_permissions(char *cmd, int *exit_code)
{
	if (access(cmd, X_OK) == 0)
		return (0);
	*exit_code = 126;
	ft_perror(cmd);
	return (1);
}

int	exec_builtin(int fd_in, int fd_out, t_builtin builtin, t_node *node, t_ms *ms)
{
	int			exit_code;
	t_cmd		cmd;

	if (builtin != BI_EXPORT)
		if (expand_node(node, ms))
			return (-1);
	create_cmd(&cmd, node);
	exit_code = run_builtin(fd_in, fd_out, builtin, &cmd, ms);
	return (exit_code);
}

pid_t	exec_fork_builtin(int fd_in, int fd_out, t_builtin builtin, t_node *node, t_ms *ms)
{
	pid_t	pid;
	t_cmd	cmd;

	if (builtin != BI_EXPORT)
		if (expand_node(node, ms))
			return (-1);
	create_cmd(&cmd, node);
	pid = fork_run_builtin(fd_in, fd_out, builtin, &cmd, ms);
	return (pid);
}

int	exec_cmd(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	t_cmd	cmd;
	int		exit_code;
	int		status;

	exit_code = 0;
	status = 0;
	pid = fork();
	if (pid == 0)
	{
		ft_close_fd(node->cfd0, node->cfd1);
		if (expand_node(node, ms))
			return (-1);
		create_cmd(&cmd, node);
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		ft_cmd_is_dot(&cmd, ms);
		ft_get_env_value(ms, cmd.path, "PATH");
		if ( \
			ft_cmd_has_slash(&cmd, &exit_code)
			|| ft_cmd_is_dotdot(&cmd, &exit_code)					// 127
			|| ft_cmd_is_dir(cmd.cmdpth, &exit_code)				// 126
			|| ft_prepend_path(&cmd.cmdpth, cmd.path, &exit_code)	// 127
			|| ft_exec_permissions(cmd.cmdpth, &exit_code))			// 126
		{
			if (exit_code == 0)
				exit_code = 1;
			ft_close_fd(fd_in, fd_out);
			terminate(ms, &cmd, exit_code);
		}
		execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_perror(cmd.args[0]);
		ft_close_fd(fd_in, fd_out);
		terminate(ms, &cmd, 0);
	}
	waitpid(pid, &status, 0);
	ms->exit_code = WEXITSTATUS(status);
	ft_close_fd(fd_in, fd_out);
	return (ms->exit_code);
}

static int	redirect_in(int *fd_in, t_node *node, t_ms *ms)
{
	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	close(*fd_in);
	*fd_in = open(node->tokens[1]->str, O_RDONLY, 0644);
	if (*fd_in < 0)
		return (ft_error(node->tokens[1]->str, "No such file or directory\n", NULL), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	redirect_out(int *fd_out, t_node *node, t_ms *ms)
{
	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	if (access(node->tokens[0]->str, W_OK) == -1)
		return (ft_error(node->tokens[1]->str, "Permission denied", NULL), EXIT_FAILURE);
	close(*fd_out);
	if (node->tokens[0]->type == TOKEN_DGREATER)
		*fd_out = open(node->tokens[1]->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_out = open(node->tokens[1]->str, O_WRONLY | O_CREAT |O_TRUNC, 0644);
	if (*fd_out < 0)
		return (ft_error(node->tokens[1]->str, "Permission denied", NULL), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	redirect_manager(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	int	exit_code;

	exit_code = EXIT_FAILURE;
	if (node->tokens == NULL)
		return (EXIT_FAILURE);
	if (node->tokens[0]->type == TOKEN_DLESS)
	{
		;//nicht fertig
	}
	else if (node->tokens[0]->type == TOKEN_LESS && node->tokens[1])
	{
		if (redirect_in(&fd_in, node, ms) > 0)
			return (1);
		exit_code = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	else if (node->tokens[0]->type == TOKEN_GREATER || node->tokens[0]->type == TOKEN_DGREATER)
	{
		if (redirect_out(&fd_out, node, ms) > 0)
			return (1);
		exit_code = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	return (exit_code);
}

int	exec_pipe(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	int		exit_code;
	int		fd_pipe[2];
	t_node	*buff;

	if (pipe(fd_pipe))
		perror(NINJASHELL);
	buff = node->left;
	while (buff->left && buff->type != NODE_COMMAND)
		buff = buff->left;
	buff->cfd0 = fd_pipe[0];
	if (node->left)
		exit_code = exec_intermediary(fd_in, fd_pipe[1], node->left, ms);
	ft_close_fd(fd_in, fd_pipe[1]);
	if (node->right)
		exit_code = exec_intermediary(fd_pipe[0], fd_out, node->right, ms);
	ft_close_fd(fd_pipe[0], fd_out);
	return (exit_code);
}

int	logical_operation_manager(t_node *node, t_ms *ms)
{
	int	exit_code;

	exit_code = 0;
	if (node->tokens[0]->type == TOKEN_DAND)
	{
		if (node->left)
			exit_code = exec_intermediary(STDIN_FILENO, STDOUT_FILENO, node->left, ms);
		if (node->right && exit_code == 0)
			exit_code = exec_intermediary(STDIN_FILENO, STDOUT_FILENO, node->right, ms);
	}
	else if (node->tokens[0]->type == TOKEN_OR)
	{
		if (node->left)
			exit_code = exec_intermediary(STDIN_FILENO, STDOUT_FILENO, node->left, ms);
		if (node->right && exit_code != 0)
			exit_code = exec_intermediary(STDIN_FILENO, STDOUT_FILENO, node->right, ms);
	}
	return (exit_code);
}

int	exec_intermediary(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	int		exit_code;
	t_builtin	builtin;

	exit_code = -1;
	if (node == NULL)
		return (0);
	builtin = is_builtin(node->tokens[0]);
	if (node->type == NODE_COMMAND && builtin != NO_BUILTIN)
		exit_code = exec_fork_builtin(fd_in, fd_out, builtin, node, ms);
	else if (node->type == NODE_COMMAND)
		exit_code = exec_cmd(fd_in, fd_out, node, ms);
	else if (node->type == NODE_REDIRECT)
		exit_code = redirect_manager(fd_in, fd_out, node, ms);
	else if (node->type == NODE_PIPE)
		exit_code = exec_pipe(fd_in, fd_out, node, ms);
	else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
		exit_code = logical_operation_manager(ms->nodes, ms);
	return (exit_code);
}

int	exec_manager(t_ms *ms)
{
	int			exit_code;
	t_builtin	builtin;
	int			std_fds[2];

	exit_code = 0;
	if (ms->nodes == NULL)
		return (-1);
	builtin = is_builtin(ms->nodes->tokens[0]);
	if (ms->nodes->type == NODE_COMMAND && builtin != NO_BUILTIN)
		ms->exit_code = exec_builtin(STDIN_FILENO, STDOUT_FILENO, builtin, ms->nodes, ms);
	else if (ms->nodes->type == NODE_COMMAND)
	{
		save_stdfds(std_fds);
		exit_code = exec_cmd(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		reset_stdfds(std_fds);
	}
	else if (ms->nodes->type == NODE_REDIRECT)
	{
		save_stdfds(std_fds);
		exit_code = redirect_manager(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		reset_stdfds(std_fds);
	}
	else if (ms->nodes->type == NODE_PIPE)
	{
		save_stdfds(std_fds);
		exit_code = exec_pipe(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		reset_stdfds(std_fds);
	}
	else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
	{
		save_stdfds(std_fds);
		exit_code = logical_operation_manager(ms->nodes, ms);
		reset_stdfds(std_fds);
	}
	return (exit_code);
}
