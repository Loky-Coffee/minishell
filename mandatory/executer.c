
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
		args[j] = ft_substr(node->tokens[j]->str, 0, node->tokens[j]->len);
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

int	execute_cmd(int fdr, int fdw, t_node *node, t_ms *ms, int exit_code)
{
	int		pid;
	t_cmd	cmd;
	int		err;
	
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
			cmd.cmdpth = ft_substr(node->tokens[0]->str, 0, node->tokens[0]->len);
		cmd.path = getenv("PATH");
		if (cmd.cmdpth[0] == '\0' || (ft_strchr(cmd.cmdpth, '/') == NULL
			&& ft_prepend_path(&cmd.cmdpth, cmd.path))
			|| ft_exec_permissions(cmd.cmdpth, &exit_code))
		{
			ft_cmd_error(NINJASHELL, cmd.args[0], exit_code);
			ft_close_fd(fdr, fdw);
			free(cmd.cmdpth);
			free_av(cmd.args);
			terminate(ms, exit_code);
		}
		err = execve(cmd.cmdpth, cmd.args, ms->envp);
		ft_close_fd(fdr, fdw);
		free(cmd.cmdpth);
		free_av(cmd.args);
		ft_perror(cmd.args[0]);
		if (err < 0)
		{
			ft_perror(NINJASHELL);
			terminate(ms, 1);
		}
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
	if (node->tokens[0] && tkn_is_operator(node->tokens[0]) == TOKEN_PIPE)
	{
		if (pipe(fdp))
			perror("ninjaSHELL");
		if (node->lft)
			status = execute(fdr, fdp[1], node->lft, ms, 0);
			// execute_cmd(fdr, fdp[1], ms->node->lft, ms, 1); // is the exit_code right here?? it only has to be 127 at the last cmd
		if (fdp[1] != STDOUT_FILENO)
			close(fdp[1]);
		if (node->rgt)
			status = execute(fdp[0], fdw, node->rgt, ms, 1);
			// execute_cmd(fdp[0], fdw, ms->node->rgt, ms, 127); // is the exit_code right here?? it only has to be 127 at the last cmd
		ft_close_fd(fdr, fdw);
		ft_close_fd(fdp[0], 0);
	}
	else
	{
		if (is_rgt && node->rgt == NULL)
			exit_code = 127;
		pid = execute_cmd(fdr, fdw, node, ms, exit_code); // is the exit_code right here?? it only has to be 127 at the last cmd
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

int	exec_manager(t_ms *ms)
{
	int	pid;
	int	status;
	
	if (ms->node == NULL)
		return (-1);
	if (ms->node->lft == NULL && ms->node->rgt == NULL && (ms->node->tokens && tkn_is_word(ms->node->tokens[0])))
	{
		pid = execute_cmd(STDIN_FILENO, STDOUT_FILENO, ms->node, ms, 1); // is the exit_code right here?? it only has to be 127 at the last cmd
		waitpid(pid, &status, 0);
	}
	else
		status = execute(STDIN_FILENO, STDOUT_FILENO, ms->node, ms, 0);
	return (status);
}
