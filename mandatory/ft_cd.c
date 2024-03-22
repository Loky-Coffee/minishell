/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 00:41:52 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/22 19:14:35 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	count_dots(t_ms *ms)
{
	int	i;
	int	dots;

	i = 0;
	dots = 0;
	if (ms->tokens->next)
		ms->tokens = ms->tokens->next;
	while (ms->tokens->str[i])
	{
		if (ms->tokens->str[i + 1] && ms->tokens->str[i] == '.' && \
		ms->tokens->str[i + 1] == '.')
		{
			dots++;
			i++;
		}
		i++;
	}
	return (dots);
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
	result = chdir(home_dir);
	if (result != 0)
		result = (ft_double_perror("cd", home_dir) ,1);
	if (full_path)
		free(full_path);
	return result;
}

static int	handle_direct_cd(t_ms *ms, char *current_cwd, char *old_cwd)
{
	int	i;
	int	dots;

	i = 0;
	if (ms->tokens->next->str)
	{
		dots = count_dots(ms);
		if (dots > 0)
		{
			while (i < dots)
			{
				chdir("..");
				i++;
			}
			printf("current_cwd%s\n", current_cwd);
			printf("old_cwd%s\n", old_cwd);
		}
		else if (chdir(ms->tokens->str) != 0)
			return (ft_double_perror("cd", ms->tokens->str), 1);
	}
	return (0);
}
/* ************************************************************************** */

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

	if (ft_strncmp(ms->tokens->str, "cd", 2) == 0 && !ms->tokens->next)
	{
		getcwd(old_cwd, sizeof(old_cwd));
		if (chdir(getenv("HOME")) != 0)
			return (ft_perror("cd"), 1);
		getcwd(current_cwd, sizeof(current_cwd));
	}
	else if (ms->tokens->next && ft_strncmp(ms->tokens->next->str, "-\0", 2) == 0)
	{
		if (ft_strlen(old_cwd) == 0)
			return (ft_perror("cd"), 1);
		getcwd(current_cwd, sizeof(current_cwd));
		if (chdir(old_cwd) != 0)
			return (ft_perror("cd"), 1);
		ft_strlcpy(old_cwd, current_cwd, ft_strlen(current_cwd) + 1);
		return (0);
	}
	else if ((ms->tokens->next && is_single_token(*ms->tokens->next->str) == TOKEN_TILDE))
		return is_tilde(old_cwd, ms);
	getcwd(current_cwd, sizeof(current_cwd));
	if (ms->tokens->next && ms->tokens->next->str)
	{
		ft_strlcpy(old_cwd, current_cwd, ft_strlen(current_cwd) + 1);
		return (handle_direct_cd(ms, current_cwd, old_cwd));
	}
	return (0);
}
