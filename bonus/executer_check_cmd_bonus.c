/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_check_cmd_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:40:34 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:40:36 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	ft_cmd_is_empty(int fd_in, int fd_out, t_cmd *cmd, t_ms *ms)
{
	int	i;

	i = 0;
	if ((cmd && cmd->cmdpth && cmd->cmdpth[0] == '\0') || cmd->cmdpth == NULL)
	{
		while (cmd->args[i])
		{
			if (cmd->args[i][0] == '\0' && cmd->args[i + 1] \
			&& cmd->args[i + 1][0] != '\0')
				ft_collaps_args(i, cmd);
			i++;
		}
		if (cmd->args == NULL || cmd->args[0] == NULL \
		|| (cmd->args[0] && cmd->args[0][0] == '\0'))
		{
			ft_close_fd(fd_in, fd_out);
			terminate(ms, cmd, 0);
		}
		free(cmd->cmdpth);
		cmd->cmdpth = ft_strdup(cmd->args[0]);
	}
}

void	ft_cmd_is_dot(t_cmd *cmd, t_ms *ms)
{
	if (cmd && cmd->cmdpth && ft_strncmp(cmd->cmdpth, ".", 2) == 0)
	{
		ft_error(cmd->cmdpth, "filename argument required", NULL);
		ft_putstr_fd(LIGHTCYAN "", 2);
		ft_putstr_fd(cmd->cmdpth, 2);
		ft_putstr_fd(": usage: ", 2);
		ft_putstr_fd(". filename [arguments]\n"RESET, 2);
		terminate(ms, cmd, 2);
	}
	if (cmd && cmd->cmdpth && ft_strncmp(cmd->cmdpth, "./", 2) == 0)
	{
		if (cmd && cmd->cmdpth && access(cmd->cmdpth, F_OK) != 0)
		{
			ft_perror(cmd->cmdpth);
			terminate(ms, cmd, 127);
		}
		if (cmd && cmd->cmdpth && access(cmd->cmdpth, X_OK) != 0)
		{
			ft_perror(cmd->cmdpth);
			terminate(ms, cmd, 126);
		}
	}
}

int	ft_cmd_is_dotdot(t_cmd *cmd, int *exit_code)
{
	if (cmd && cmd->cmdpth && ft_strncmp(cmd->cmdpth, "..", 3) == 0)
	{
		ft_error(cmd->cmdpth, "command not found", NULL);
		*exit_code = 127;
		return (1);
	}
	return (0);
}

int	ft_cmd_has_slash(t_cmd *cmd, int *exit_code)
{
	if (cmd && cmd->cmdpth && cmd->cmdpth[0] == '/')
	{
		if (access(cmd->cmdpth, F_OK | X_OK) == 0)
			return (0);
		*exit_code = 127;
		return (ft_error(cmd->cmdpth, "No such file or directory", NULL), 1);
	}
	return (0);
}

int	ft_cmd_is_dir(char *cmd, int *exit_code)
{
	struct stat	file_stat;

	if (cmd && ft_strchr(cmd, '/') == NULL )
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
