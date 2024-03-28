/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 16:47:45 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/28 18:40:17 by nmihaile         ###   ########.fr       */
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

// static t_builtin	is_builtin(t_token *token, t_cmd *cmd, t_ms *ms)
static t_builtin	is_builtin(t_token *token)
{
	if (ft_strncmp_ignorecase(token->str, "echo", 5) == 0)
		// exec_builtin(BI_ECHO, cmd, ms);
		return (BI_ECHO);
	if (ft_strncmp_ignorecase(token->str, "cd", 3) == 0)
		// exec_builtin(BI_CD, cmd, ms);
		return (BI_CD);
	if (ft_strncmp_ignorecase(token->str, "pwd", 4) == 0)
		// exec_builtin(BI_PWD, cmd, ms);
		return (BI_PWD);
	if (ft_strncmp_ignorecase(token->str, "export", 7) == 0)
		// exec_builtin(BI_EXPORT, cmd, ms);
		return (BI_EXPORT);
	if (ft_strncmp_ignorecase(token->str, "unset", 6) == 0)
		// exec_builtin(BI_UNSET, cmd, ms);
		return (BI_UNSET);
	if (ft_strncmp_ignorecase(token->str, "env", 4) == 0)
		// exec_builtin(BI_ENV, cmd, ms);
		return (BI_ENV);
	if (ft_strncmp_ignorecase(token->str, "exit", 5) == 0)
		// exec_builtin(BI_EXIT, cmd, ms);
		return (BI_EXIT);
	return (NO_BUILTIN);
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
		// cmd.tokens = node->tokens;
		// cmd.args = create_cmd_args(node);
		// if (cmd.args == NULL)
		// 	cmd.cmdpth = ft_strdup("");
		// else
		// 	cmd.cmdpth = ft_strdup(node->tokens[0]->str);
		create_cmd(&cmd, node);

		// Buildin check
		// is_builtin(node->tokens[0]);

		ft_get_env_value(ms, cmd.path, "PATH");
		if (cmd.cmdpth[0] == '\0'
			|| ft_cmd_is_dir(cmd.cmdpth, &exit_code)
			|| ft_prepend_path(&cmd.cmdpth, cmd.path)
			|| ft_exec_permissions(cmd.cmdpth, &exit_code))
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
	if (node->tokens[0] && node->tokens[0]->type == TOKEN_PIPE)
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
			printf("###########################################\n\n\n\n\n\n\n\n\n\n");
			int	fdr_new;
			fdr_new = open(node->tokens[1]->str, O_RDONLY | O_CREAT, 0644);
			if (fdr_new != -1)
			{
				dup2(fdr_new, STDIN_FILENO);
				// close(fdr_new);
				close (fdr);
				fdr = fdr_new;
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
		expand_node(node);
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
// 	dup2(STDIN_FILENO, fds[0]);
// 	dup2(STDOUT_FILENO, fds[1]);
// }

// static void set_stdfds(int *fds)
// {
// 	dup2(fds[0], STDIN_FILENO);
// 	dup2(fds[1], STDOUT_FILENO);
// 	close(fds[0]);
// 	close(fds[1]);
// }

int	exec_manager(t_ms *ms)
{
	int			pid;
	int			status;
	t_cmd		cmd;
	t_builtin	builtin;
	// int	std_fds[2];
	// struct stat stdin;
	// struct stat stdout;

	// fstat(STDIN_FILENO, &stdin);
	// fstat(STDIN_FILENO, &stdin);
	// save_stdfds(std_fds);
	if (ms->nodes == NULL)
		return (-1);
	if (ms->nodes->left == NULL && ms->nodes->right == NULL && (ms->nodes->tokens && is_word(ms->nodes->tokens[0]->str)))
	{
		builtin = is_builtin(ms->nodes->tokens[0]);
		if (builtin)
		{
			if (builtin != BI_EXPORT)
				expand_node(ms->nodes);
			create_cmd(&cmd, ms->nodes);
			status = exec_builtin(builtin, &cmd, ms, 0);
			free_cmd(&cmd);
			return (status);
		}
		expand_node(ms->nodes);
		pid = execute_cmd(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms, 127);
		waitpid(pid, &status, 0);
	}
	else
		status = execute(STDIN_FILENO, STDOUT_FILENO, ms->nodes, ms, 0);
	return (status);
	// set_stdfds(std_fds);
}
