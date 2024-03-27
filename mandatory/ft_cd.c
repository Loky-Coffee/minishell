/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 00:41:52 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/25 19:30:56 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_chdir(t_ms *ms, char *dir)
{
	int i;
	char str[PATH_MAX];
	char *new_pwd;

	i = 0;
	if (chdir(dir) != 0)
		return (ft_perror("cd"), 1);
	if (!getcwd(str, PATH_MAX))
		return (1);
	while (ms->envp[i] && ft_strncmp(ms->envp[i],"PWD", 3) != 0)
		i++;
	if (ms->envp[i] == NULL)
		return (1);
	new_pwd = malloc(4 + ft_strlen(str) + 1);
	if (new_pwd == NULL)
		return (1);
	strcpy(new_pwd, "PWD=");
	strcat(new_pwd, str);
	free(ms->envp[i]);
	ms->envp[i] = ft_strdup(new_pwd);
	return (free(new_pwd), 0);
}

static int is_tilde(char *old_cwd, t_ms *ms)
{
	char *home_dir;
	static char *full_path;
	int result;

	result = 0;
	getcwd(old_cwd, PATH_MAX);
	home_dir = getenv("HOME");
	if (!home_dir)
		return (ft_perror("cd"),1);
	if (ms->tokens->next->next && is_word(ms->tokens->next->next->str))
	{
		full_path = ft_strjoin(home_dir, ms->tokens->next->next->str);
		home_dir = full_path;
	}
	result = ft_chdir(ms, home_dir);
	if (result != 0)
		result = (ft_double_perror("cd", home_dir) ,1);
	free(full_path);
	full_path = NULL;
	return (result);
}

// Changes the current directory based on the provided arguments.
// If the next token is "-",changes to the previous directory stored in old_cwd.
// If there is no next token, it changes to the home directory by the HOME
// environment variable. If there is a token and it's not "-", it calls ft_cd2
// function to handle the dir change based on the token string. It also updates
// the current and old directory paths stored in current_cwd and old_cwd.
int	ft_cd(t_ms *ms)
{
	static char	current_cwd[PATH_MAX] = {0};
	static char	old_cwd[PATH_MAX] = {0};
	static char	home[PATH_MAX] = {0};

	if (ft_strncmp(ms->tokens->str, "cd\0", 3) == 0 && !ms->tokens->next)
	{
		ft_get_env_value(ms, old_cwd, "PWD");
		ft_get_env_value(ms, home, "HOME");
		if (ft_chdir(ms, home) != 0)
			return (ft_perror("cd"), 1);
		ft_get_env_value(ms, current_cwd, "PWD");
		return (0);
	}
	else if (ms->tokens->next && ft_strncmp(ms->tokens->next->str, "-\0", 2) == 0)
	{
		if (ft_strlen(old_cwd) == 0)
			return (ft_perror("cd"), 1);
		ft_get_env_value(ms, current_cwd, "PWD");
		if (ft_chdir(ms, old_cwd) != 0)
			return (ft_perror("cd"), 1);
		ft_strlcpy(old_cwd, current_cwd, ft_strlen(current_cwd) + 1);
		return (0);
	}
	else if ((ms->tokens->next && is_single_token(*ms->tokens->next->str) == TOKEN_TILDE))
		return (is_tilde(old_cwd, ms));
	else
	{
		ft_get_env_value(ms, old_cwd, "PWD");
		ft_chdir(ms, ms->tokens->next->str);
	}
	return (0);
}
