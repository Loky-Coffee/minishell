/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/16 21:57:30 by nmihaile         ###   ########.fr       */
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

static int	create_cmd(t_cmd *cmd, t_node *node)
{
	cmd->tokens = node->tokens;
	cmd->args = create_cmd_args(node);
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
		*exit_code = 127;
		return (1);
	}
	return (0);
}

static int	ft_cmd_is_dir(char *cmd, int *exit_code)
{
	struct stat	file_stat;

	if (ft_strchr(cmd, '/') == NULL)
		return (0);
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

static int	ft_exec_permissions(char *cmd, int *exit_code)
{
	if (access(cmd, X_OK) == 0)
		return (0);
	*exit_code = 126;
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

pid_t	exec_cmd(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	t_cmd	cmd;
	int		exit_code;

	exit_code = 0;
	pid = fork();
	if (pid == 0)
	{
		if (expand_node(node, ms))
			return (-1);
		create_cmd(&cmd, node);
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		ft_close_fd(fd_in, fd_out);
		ft_cmd_is_dot(&cmd, ms);
		ft_get_env_value(ms, cmd.path, "PATH");
		if (cmd.cmdpth[0] != '/'
			&& (cmd.cmdpth[0] == '\0'
			|| ft_cmd_is_dotdot(&cmd, &exit_code)					// 127
			|| ft_cmd_is_dir(cmd.cmdpth, &exit_code)				// 126
			|| ft_prepend_path(&cmd.cmdpth, cmd.path, &exit_code)	// 127
			|| ft_exec_permissions(cmd.cmdpth, &exit_code)))		// 126
		{
			if (exit_code == 0)
				exit_code = 1;
			ft_cmd_error(NINJASHELL, cmd.args[0], exit_code);
			// ft_close_fd(fd_in, fd_out);
			terminate(ms, &cmd, exit_code);
		}
// check_fds();
// close(5);
		execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_perror(cmd.args[0]);
		// ft_close_fd(fd_in, fd_out);
		terminate(ms, &cmd, 1);
	}
	ft_close_fd(fd_in, fd_out);
	return (pid);
}

static int	redirect_in(int *fd_in, t_node *node, t_ms *ms)
{
	// int	fd_in_new;

	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	// fprintf(stderr, "---------->redirect_in |%i|\n", *fd_in);
	close(*fd_in);
	*fd_in = open(node->tokens[1]->str, O_RDONLY, 0644);
	if (*fd_in < 0)
		return (ft_perror(node->tokens[1]->str), EXIT_FAILURE);
	// close(*fd_in);
	// *fd_in = dup(fd_in_new);
	// close(fd_in_new);
	return (EXIT_SUCCESS);
}

static int	redirect_out(int *fd_out, t_node *node, t_ms *ms)
{
	int	fd_out_new;

	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	if (node->tokens[0]->type == TOKEN_DGREATER)
		fd_out_new = open(node->tokens[1]->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd_out_new = open(node->tokens[1]->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out_new < 0)
		return (ft_perror(node->tokens[1]->str), EXIT_FAILURE);
	close(*fd_out);
	*fd_out = dup(fd_out_new);
	close(fd_out_new);
	return (EXIT_SUCCESS);	
}

static pid_t	redirect_manager(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;

	pid = EXIT_FAILURE;
	if (node->tokens == NULL)
		return (EXIT_FAILURE);
	if (node->tokens[0]->type == TOKEN_DLESS)
	{
		;
	}
	else if (node->tokens[0]->type == TOKEN_LESS && node->tokens[1])
	{
		redirect_in(&fd_in, node, ms);
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	else if (node->tokens[0]->type == TOKEN_GREATER || node->tokens[0]->type == TOKEN_DGREATER)
	{
		redirect_out(&fd_out, node, ms);
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	// if (node->left->type == NODE_REDIRECT)
	// 	pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	return (pid);
}

pid_t	exec_pipe(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	int		fd_pipe[2];

	if (pipe(fd_pipe))
		perror(NINJASHELL);

	// fprintf(stderr, "---------->pipe fd_in|%i|  fd_out|%i|\n", fd_in, fd_out);
	// fprintf(stderr, "---------->pipe read|%i|  write|%i|\n", fd_pipe[0], fd_pipe[1]);


	if (node->left)
	{
		close(fd_pipe[0]);
		pid = exec_intermediary(fd_in, fd_pipe[1], node->left, ms);
	}
	ft_close_fd(fd_in, fd_pipe[1]);
	if (node->right)
	{
		close(fd_pipe[1]);
		pid = exec_intermediary(fd_pipe[0], fd_out, node->right, ms);
	}
	ft_close_fd(fd_pipe[0], fd_out);
	return (pid);
}

// int	exec_logical_operation(t_node *node, t_ms *ms)
// {
// 	return (0);
// }

pid_t	exec_intermediary(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t		pid;
	t_builtin	builtin;

	pid = -1;
	if (node == NULL)
		return (0);
	builtin = is_builtin(node->tokens[0]);
	if (node->type == NODE_COMMAND && builtin != NO_BUILTIN)
		pid = exec_fork_builtin(fd_in, fd_out, builtin, node, ms);
	else if (node->type == NODE_COMMAND)
		pid = exec_cmd(fd_in, fd_out, node, ms);
	else if (node->type == NODE_REDIRECT)
		pid = redirect_manager(fd_in, fd_out, node, ms);
	else if (node->type == NODE_PIPE)
		pid = exec_pipe(fd_in, fd_out, node, ms);
	// else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
	// 	exit_code = exec_logical_operation(ms->nodes, ms);
	return (pid);
}

int	exec_manager(t_ms *ms)
{
	pid_t		pid;
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
		pid = exec_cmd(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		waitpid(pid, &exit_code, 0);
		ms->exit_code = WEXITSTATUS(exit_code);
	}
	else if (ms->nodes->type == NODE_REDIRECT)
	{
		save_stdfds(std_fds);
		pid = redirect_manager(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		waitpid(pid, &exit_code, 0);
		ms->exit_code = WEXITSTATUS(exit_code);
		reset_stdfds(std_fds);
	}
	else if (ms->nodes->type == NODE_PIPE)
	{
		save_stdfds(std_fds);
		pid = exec_pipe(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		waitpid(pid, &exit_code, 0);
		while (waitpid(-1, NULL, 0) > 0)
			;
		ms->exit_code = WEXITSTATUS(exit_code);
		reset_stdfds(std_fds);
	}
	// else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
	// {
	// 	save_stdfds(std_fds);
	// 	exit_code = exec_logical_operation(ms->nodes, ms);
	// 	reset_stdfds(std_fds);
	// }
	return (ms->exit_code);
}
