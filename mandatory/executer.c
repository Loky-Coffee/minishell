/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/15 18:04:13 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

static void	ft_check_cmd_is_dot(t_cmd *cmd, t_ms *ms)
{
	if (ft_strncmp(cmd->cmdpth, ".", 2) == 0)
	{
		ft_error(cmd->cmdpth, "filename argument required", NULL);
		ft_putstr_fd(LIGHTRED, 2);
		ft_putstr_fd(cmd->cmdpth, 2);
		ft_putstr_fd(": usage: ", 2);
		ft_putstr_fd(". filename [arguments]\n"RESET, 2);
		terminate(ms, cmd, 2);
	}
}

int	exec_builtin(t_builtin builtin, t_node *node, t_ms *ms)
{
	int			exit_code;
	t_cmd		cmd;

	create_cmd(&cmd, node);
	if (builtin != BI_EXPORT)
		if (expand_node(ms->nodes, ms))
			return (-1);
	exit_code = run_builtin(builtin, &cmd, ms, 0);
	return (exit_code);
}

int	exec_cmd(t_node *node, t_ms *ms)
{
	int			pid;
	int			exit_code;
	t_cmd		cmd;
	t_builtin	builtin;

	exit_code = 0;
	builtin = is_builtin(node->tokens[0]);
	create_cmd(&cmd, node);
	if (builtin == NO_BUILTIN)
	{
		pid = fork();
		if (pid == 0)
		{
			ft_check_cmd_is_dot(&cmd, ms);
			ft_get_env_value(ms, cmd.path, "PATH");
			if (cmd.cmdpth[0] != '/' && (cmd.cmdpth[0] == '\0'
				|| ft_strncmp(cmd.cmdpth, "..", 3) == 0
				|| ft_cmd_is_dir(cmd.cmdpth, &exit_code)
				|| ft_prepend_path(&cmd.cmdpth, cmd.path)
				|| ft_exec_permissions(cmd.cmdpth, &exit_code)))
			{
				ft_cmd_error(NINJASHELL, cmd.args[0], exit_code);
				terminate(ms, &cmd, exit_code);
			}
			execve(cmd.cmdpth, cmd.args, ms->envp);
			ft_perror(NINJASHELL);
			ft_perror(cmd.args[0]);
			terminate(ms, &cmd, 1);
		}
		return (pid);
	}
	return (-1);
}
// int	exec_redirect(t_node *node, t_ms *ms)
// {
// 	return (0);
// }
int	exec_pipe(t_node *node, t_ms *ms)
{
	int	fd_pipe[2];

	if (pipe(fd_pipe))
		perror(NINJASHELL);
	if (node->left)
	{
		dup2(fd_pipe[1], STDOUT_FILENO);
		if (node->left->type == NODE_COMMAND)
			exec_cmd(node->left, ms);
	}
	if (node->right)
	{
		dup2(fd_pipe[0], STDIN_FILENO);
		if (node->right->type == NODE_COMMAND)
			exec_cmd(node->right, ms);
	}
	close(fd_pipe[0]);
	return (0);
}
// int	exec_logical_operation(t_node *node, t_ms *ms)
// {

// 	return (0);
// }

int	exec_manager(t_ms *ms)
{
	int	pid;
	int	exit_code;
	t_builtin	builtin;
	int			std_fds[2];
	exit_code = 0;

	if (ms->nodes == NULL)
		return (-1);
	builtin = is_builtin(ms->nodes->tokens[0]);
	if (ms->nodes->type == NODE_COMMAND && builtin != NO_BUILTIN)
		exit_code = exec_builtin(builtin, ms->nodes, ms);
	else if (ms->nodes->type == NODE_COMMAND)
	{
		pid = exec_cmd(ms->nodes, ms);
		waitpid(pid, &exit_code, 0);
		ms->exit_code = WEXITSTATUS(exit_code);
	}
	// else if (ms->nodes->type == NODE_REDIRECT)
	// 	exit_code = exec_redirect(ms->nodes, ms);
	else if (ms->nodes->type == NODE_PIPE)
	{
		save_stdfds(std_fds);
		exit_code = exec_pipe(ms->nodes, ms);
		reset_stdfds(std_fds);
	}
	// else if (ms->nodes->type == NODE_AND || ms->nodes->type == NODE_OR)
	// 	exit_code = exec_logical_operation(ms->nodes, ms);
	// reset_stdfds(std_fds);
	return (exit_code);
}
