/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/30 18:30:07 by nmihaile         ###   ########.fr       */
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
// 	else
// 		fprintf(stderr, "open fds: %d\n", open_fd_count);
// }

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

void	set_exit_code(int status, t_ms *ms)
{
	if (WIFSIGNALED(status))
		ms->exit_code = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		ms->exit_code = WEXITSTATUS(status);	
}

void	set_hd_exit_code(int status, t_ms *ms)
{
	set_exit_code(status, ms);
	ms->hd_interupt = 1;
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

static t_builtin	is_builtin(t_token *token)
{
	if (cmd_is_echo_ninjashell(token))
		return (BI_ECHO_NINJASHELL);
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
		return (1);
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

int	create_subshell_cmd(t_cmd *cmd, t_node *node, t_ms *ms)
{
	cmd->tokens = node->tokens;
	cmd->cmdpth = ft_strdup(ms->av[0]);
	cmd->args = (char **)ft_calloc(4, sizeof(char *));
	if (cmd->args == NULL)
		return (1);
	cmd->args[0] = ft_strdup("minishell");
	cmd->args[1] = ft_strdup("-c");
	cmd->args[2] = ft_strdup(node->tokens[0]->str);
	cmd->args[2][0] = ' ';
	cmd->args[2][ft_strlen(cmd->args[2]) - 1] = ' ';
	return (0);
}

static void	ft_collaps_args(int i, t_cmd *cmd)
{
	while (cmd->args[i + 1])
	{
		cmd->args[i] = cmd->args[i + 1];
		i++;
	}
	cmd->args[i] = NULL;	
}

static void	ft_cmd_is_empty(int fd_in, int fd_out, t_cmd *cmd, t_ms *ms)
{
	int	i;
	
	i = 0;
	if (cmd->cmdpth[0] == '\0')
	{
		while (cmd->args[i])
		{
			if (cmd->args[i][0] == '\0' && cmd->args[i + 1] && cmd->args[i + 1][0] != '\0')
				ft_collaps_args(i, cmd);
			i++;
		}
		if (cmd->args == NULL || (cmd->args[0] && cmd->args[0][0] == '\0'))
		{
			ft_close_fd(fd_in, fd_out);
			terminate(ms, cmd, 0);		
		}
		free(cmd->cmdpth);
		cmd->cmdpth = ft_strdup(cmd->args[0]);
	}
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
	if (ft_strncmp(cmd->cmdpth, "./", 2) == 0)
	{
		if (access(cmd->cmdpth, F_OK) != 0)
		{
			ft_perror(cmd->cmdpth);
			terminate(ms, cmd, 127);
		}		
		if (access(cmd->cmdpth, X_OK) != 0)
		{
			ft_perror(cmd->cmdpth);
			terminate(ms, cmd, 126);
		}		
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

	if (ft_strchr(cmd, '/') == NULL ) // || ft_strchr(cmd, '.') == NULL
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


pid_t	exec_cmd(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	t_cmd	cmd;
	int		exit_code;

	exit_code = 0;
	pid = fork();
	if (pid == 0)
	{
		set_echoctl(1);
		set_signal_handler(SIGINT, SIG_DFL);
		set_signal_handler(SIGQUIT, SIG_DFL);
		ft_close_fd(node->cfd0, node->cfd1);
		if (expand_node(node, ms))
			return (-1);
		create_cmd(&cmd, node);
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		ft_cmd_is_empty(fd_in, fd_out, &cmd, ms);
		ft_cmd_is_dot(&cmd, ms);
		ft_get_env_value(ms, cmd.path, "PATH");
		if (ft_cmd_is_dir(cmd.cmdpth, &exit_code)
			|| ft_cmd_has_slash(&cmd, &exit_code)
			|| ft_cmd_is_dotdot(&cmd, &exit_code)
			|| ft_prepend_path(&cmd.cmdpth, cmd.path, &exit_code)
			|| ft_exec_permissions(cmd.cmdpth, &exit_code))
		{
			if (exit_code == 0)
				exit_code = 1;
			ms->exit_code = exit_code;
			ft_close_fd(fd_in, fd_out);
			terminate(ms, &cmd, exit_code);
		}
		execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_perror(cmd.args[0]);
		ft_close_fd(fd_in, fd_out);
		terminate(ms, &cmd, 0); /// HAEEE wieso 0?????
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
		ft_close_fd(node->cfd0, node->cfd1);		// ?? do we need that ??
		ft_close_fd(fd_in, fd_out);					// ?? do we need that ??
		if (expand_node(node, ms))					/// brauchen wir das hier schon ?????
			return (-1);
		create_subshell_cmd(&cmd, node, ms);
		set_echoctl(1);
		set_signal_handler(SIGINT, SIG_DFL);
		set_signal_handler(SIGQUIT, SIG_DFL);
		execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_perror(cmd.args[0]);
		ft_close_fd(fd_in, fd_out);
		terminate(ms, &cmd, 0);						// muss hier nicht terminate 1 hin
	}
	ft_close_fd(fd_in, fd_out);
	return (pid);
}

void	execute_heredoc(int *fd_in, int *fd_out, char *lim, t_ms *ms)
{
	int	pid;
	int	status;
	int	fd_pipe[2];

	status = 0;
	if (pipe(fd_pipe))
		perror(NINJASHELL);
	ft_close_fd(*fd_in, 0);
	*fd_in = dup(fd_pipe[0]);
	pid = fork();
	if (pid == -1)
		perror(NINJASHELL);
	if (pid == 0)
	{
		set_signal_handler(SIGINT, SIG_DFL);
		status = ft_heredoc(fd_pipe[1], lim);
		ft_close_fd(fd_pipe[0], fd_pipe[1]);
		ft_close_fd(*fd_in, *fd_out);
		terminate(ms, NULL, status);
	}
	ft_close_fd(fd_pipe[0], fd_pipe[1]);
	waitpid(pid, &status, 0);
	set_hd_exit_code(status, ms);
}

static int	redirect_in(int *fd_in, t_node *node, t_ms *ms)
{
	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	close(*fd_in);
	*fd_in = open(node->tokens[1]->str, O_RDONLY, 0644);
	if (*fd_in < 0)
		return (ft_error(node->tokens[1]->str, "No such file or directory", NULL), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	redirect_out(int *fd_out, t_node *node, t_ms *ms)
{
	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	close(*fd_out);
	if (node->tokens[0]->type == TOKEN_DGREATER)
		*fd_out = open(node->tokens[1]->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_out = open(node->tokens[1]->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out < 0)
		return (ft_error(node->tokens[1]->str, "Permission denied", NULL), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static pid_t	redirect_manager(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;

	pid = EXIT_SUCCESS;
	if (node->tokens == NULL)
		return (EXIT_FAILURE);
	if (node->tokens[0]->type == TOKEN_DLESS)
	{
		execute_heredoc(&fd_in, &fd_out, node->tokens[1]->str, ms);
		if (node->left && (node->left->type == NODE_REDIRECT || node->left->type == NODE_COMMAND || node->left->type == NODE_SUBSHELL))	// node->left->type == NODE_SUBSHELL
			pid = exec_intermediary(fd_in, fd_out, node->left, ms);
		else
			close(fd_in);
	}
	else if (node->tokens[0]->type == TOKEN_LESS && node->tokens[1])
	{
		if (redirect_in(&fd_in, node, ms) > 0)
			return (1);
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	else if (node->tokens[0]->type == TOKEN_GREATER || node->tokens[0]->type == TOKEN_DGREATER)
	{
		if (redirect_out(&fd_out, node, ms) > 0)
			return (1);
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	return (pid);
}

pid_t	exec_pipe(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	int		fd_pipe[2];
	t_node	*buff;

	if (pipe(fd_pipe))
		perror(NINJASHELL);
	buff = node->left;
	while (buff->left && buff->type != NODE_COMMAND && buff->type != NODE_SUBSHELL)	// buff->type != NODE_SUBSHELL
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

int	exec_interm_wait(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;
	int		status;
	
	pid = exec_intermediary(fd_in, fd_out, node, ms);
	waitpid(pid, &status, 0);
	set_exit_code(status, ms);
	return (ms->exit_code);
}

int	logical_operator_manager(t_node *node, t_ms *ms)
{
	int	ec;

	ec = 0;
	if (node->type == NODE_AND)
	{
		if (node->left)
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->left, ms);
		if (node->right && ms->exit_code == 0)
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->right, ms);
	}
	else if (node->type == NODE_OR)
	{
		if (node->left)
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->left, ms);
		if (node->right && ms->exit_code != 0)
			ec = exec_interm_wait(STDIN_FILENO, STDOUT_FILENO, node->right, ms);
	}
	return (ec);
}

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
	else if (node->type == NODE_SUBSHELL)
		pid = exec_subshell(fd_in, fd_out, node, ms);
	else if (node->type == NODE_REDIRECT)
		pid = redirect_manager(fd_in, fd_out, node, ms);
	else if (node->type == NODE_PIPE)
		pid = exec_pipe(fd_in, fd_out, node, ms);
	else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
		pid = logical_operator_manager(ms->nodes, ms);
	return (pid);
}

int	exec_manager(t_ms *ms)
{
	pid_t		pid;
	int			status;
	t_builtin	builtin;
	int			std_fds[2];

	status= 0;
	if (ms->nodes == NULL)
		return (-1);
	builtin = is_builtin(ms->nodes->tokens[0]);
	if (ms->nodes->type == NODE_COMMAND && builtin != NO_BUILTIN)
		ms->exit_code = exec_builtin(STDIN_FILENO, STDOUT_FILENO, builtin, ms->nodes, ms);
	else if (ms->nodes->type == NODE_COMMAND)
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
	else if (ms->nodes->type == NODE_REDIRECT)
	{
		save_stdfds(std_fds);
		pid = redirect_manager(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms);
		if (pid > 255)
		{
			waitpid(pid, &status, 0);
			set_exit_code(status, ms);
		}
		else
			ms->exit_code = 1;
		reset_stdfds(std_fds);
	}
	else if (ms->nodes->type == NODE_PIPE)
	{
		save_stdfds(std_fds);
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
		reset_stdfds(std_fds);
	}
	else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
	{
		save_stdfds(std_fds);
		logical_operator_manager(ms->nodes, ms);
		reset_stdfds(std_fds);
	}
	set_echoctl(0);				// fixes wc ctrl-c: if you just have wc and you use ctrl-c it returns the shell to hide ctrl-c output again
	return (ms->exit_code);
}
