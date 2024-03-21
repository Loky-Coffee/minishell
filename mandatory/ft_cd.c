/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 00:41:52 by aalatzas          #+#    #+#             */
/*   Updated: 2024/03/21 08:56:56 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Counts the number of consecutive dots in the token string.
// It moves to the next token if it exists and then iterates the characters
// of the token string. If two dots are found, it increments the dot
// counter. Finally, it returns the total number of consecutive dots found.
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

// Changes the current directory based on the token string.
// If the next token exists and is not NULL, it counts the number of  dots
// using the count_dots function. If there are dots, it moves up the dir tree
// by calling chdir("..")for each dot.If there are no dots,it directly changes
// the directory to the path specified by the token string using chdir.
static int	ft_cd2(t_ms *ms)
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
		}
		else if (chdir(ms->tokens->str) != 0)
			return (perror("cd Fehler"), 1);
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
	const char	*home_dir;

	if (ms->tokens->next && ft_strncmp(ms->tokens->next->str, "-", 1) == 0)
	{
		if (strlen(old_cwd) == 0)
			return (perror("cd"), 1);
		getcwd(current_cwd, sizeof(current_cwd));
		if (chdir(old_cwd) != 0)
			return (perror("cd"), 1);
		ft_strlcpy(old_cwd, current_cwd, ft_strlen(current_cwd) + 1);
		return (0);
	}
	getcwd(current_cwd, sizeof(current_cwd));
	ft_strlcpy(old_cwd, current_cwd, ft_strlen(current_cwd) + 1);
	if (ms->tokens->next && ms->tokens->next->str)
		ft_cd2(ms);
	else if (!ms->tokens->next)
	{
		home_dir = getenv("HOME");
		if (chdir(home_dir) != 0)
			return (perror("cd Fehler"), 1);
	}
	return (0);
}
