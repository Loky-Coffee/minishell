/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 00:41:52 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/09 07:44:58 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_cd2(t_cmd *cmd, t_ms *ms, char *old_cwd, char *current_cwd);

static int	ft_chdir(t_ms *ms, char *dir)
{
	int		i;
	char	str[FT_PATH_MAX];
	char	*new_pwd;

	i = 0;
	if (chdir(dir) != 0)
		return (1);
	if (!getcwd(str, FT_PATH_MAX))
		return (1);
	while (ms->envp[i] && ft_strncmp(ms->envp[i], "PWD", 3) != 0)
		i++;
	if (ms->envp[i] == NULL)
		return (1);
	new_pwd = ft_calloc(4 + ft_strlen(str) + 1, sizeof(char));
	if (new_pwd == NULL)
		return (1);
	strcpy(new_pwd, "PWD=");
	strcat(new_pwd, str);
	free(ms->envp[i]);
	ms->envp[i] = ft_strdup(new_pwd);
	return (free(new_pwd), 0);
}

static int	is_tilde(char *old_cwd, t_cmd *cmd, t_ms *ms)
{
	char		*home_dir;
	static char	*full_path;
	int			result;

	result = 0;
	getcwd(old_cwd, FT_PATH_MAX);
	home_dir = getenv("HOME");
	if (!home_dir)
		return (ft_perror("cd"), 1);
	if (cmd->tokens[0]->next->next && is_word(cmd->tokens[0]->next->next->str))
	{
		full_path = ft_strjoin(home_dir, cmd->tokens[0]->next->next->str);
		home_dir = full_path;
	}
	result = ft_chdir(ms, home_dir);
	if (result != 0)
		result = (ft_error("cd", home_dir, NULL), 1);
	free(full_path);
	full_path = NULL;
	return (result);
}

int	ft_cd(t_cmd *cmd, t_ms *ms)
{
	static char	current_cwd[FT_PATH_MAX] = {0};
	static char	old_cwd[FT_PATH_MAX] = {0};
	static char	home[FT_PATH_MAX] = {0};

	if (ft_strncmp((cmd->tokens[0])->str, "cd\0", 3) == 0 \
	&& !cmd->tokens[0]->next)
	{
		ft_get_env_value(ms, old_cwd, "PWD");
		ft_get_env_value(ms, home, "HOME");
		if (ft_chdir(ms, home) != 0)
			return (ft_perror("cd"), 1);
		ft_get_env_value(ms, current_cwd, "PWD");
		return (0);
	}
	else
		return (ft_cd2(cmd, ms, old_cwd, current_cwd));
	return (0);
}

static int	ft_cd2(t_cmd *cmd, t_ms *ms, char *old_cwd, char *current_cwd)
{
	if (cmd->tokens[0]->next \
	&& ft_strncmp(cmd->tokens[0]->next->str, "-\0", 2) == 0)
	{
		if (ft_strlen(old_cwd) == 0)
			return (ft_error("cd", "OLDPWD not set", NULL), 1);
		ft_get_env_value(ms, current_cwd, "PWD");
		if (ft_chdir(ms, old_cwd) != 0)
			return (ft_error("cd", old_cwd, NULL), 1);
		printf("%s\n", old_cwd);
		ft_strlcpy(old_cwd, current_cwd, ft_strlen(current_cwd) + 1);
		return (0);
	}
	else if ((cmd->tokens[0]->next \
	&& *cmd->tokens[0]->next->str == '~'))
		return (is_tilde(old_cwd, cmd, ms));
	else
	{
		ft_get_env_value(ms, old_cwd, "PWD");
		if (ft_chdir(ms, cmd->tokens[0]->next->str) != 0)
		{
			ft_error("cd", cmd->tokens[0]->next->str, strerror(errno));
			return (1);
		}
	}
	return (0);
}
