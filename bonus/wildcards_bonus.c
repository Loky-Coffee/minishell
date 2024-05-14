/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:44:34 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:44:35 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

static void	compress_wildcard_pattern(char *pattern)
{
	char	*cur;
	char	*res;

	if (pattern == NULL || *pattern == '\0')
		return ;
	cur = pattern;
	res = pattern;
	*res++ = *cur++;
	while (*cur)
	{
		if (*cur != '*' || *(res - 1) != '*')
			*res++ = *cur;
		cur++;
	}
	*res = '\0';
}

static int	init_mat(char mat[2048][2048], const char *str, const char *pattern)
{
	size_t	i;
	size_t	j;

	if ((str == NULL && pattern == NULL) || ft_strncmp(pattern, "*", 2) == 0)
		return (1);
	mat[0][0] = 0;
	mat[0][1] = 0;
	mat[1][0] = 0;
	ft_strlcpy(&mat[0][2], pattern, 2048 - 2);
	i = 2;
	j = 0;
	while (str[j])
		mat[i++][0] = str[j++];
	mat[1][1] = 1;
	place_pattern(mat, pattern);
	i = 0;
	while (i < ft_strlen(str))
		mat[i++ + 2][1] = 0;
	return (0);
}

static int	is_matching(size_t i, size_t j, char *str, char *pattern)
{
	char	mat[2048][2048];

	if (init_mat(mat, str, pattern))
		return (1);
	while (i < ft_strlen(str) + 2)
	{
		j = 2;
		while (j < ft_strlen(pattern) + 2)
		{
			if (mat[i][0] == mat[0][j] || mat[0][j] == '?')
				mat[i][j] = mat[i - 1][j - 1];
			else if (mat[0][j] == '*')
			{
				if (mat[i][j - 1] == 1)
					mat[i][j] = 1;
				else
					mat[i][j] = mat[i - 1][j];
			}
			else
				mat[i][j] = 0;
			j++;
		}
		i++;
	}
	return (mat[--i][--j]);
}

static int	expand_pattern(t_epv *epv, char *pattern, char *expstr)
{
	if (has_wildcards(pattern))
		return (1);
	epv->dir = opendir(".");
	if (epv->dir == NULL)
		return (ft_strlcat(expstr, pattern, FT_PATH_MAX), 1);
	ft_strlcpy(epv->pattern, pattern, FT_PATH_MAX);
	compress_wildcard_pattern(epv->pattern);
	epv->count = 0;
	epv->entry = readdir(epv->dir);
	while (epv->entry)
	{
		if (epv->entry->d_name[0] != '.'
			&& is_matching(2, 2, epv->entry->d_name, epv->pattern))
		{
			if (epv->count)
				ft_strlcat(expstr, " ", FT_PATH_MAX);
			ft_strlcat(expstr, epv->entry->d_name, FT_PATH_MAX);
			epv->count++;
		}
		epv->entry = readdir(epv->dir);
	}
	if (!epv->count)
		return (closedir(epv->dir), 1);
	return (closedir(epv->dir), 0);
}

int	expand_wildcard(t_token *token)
{
	int		i;
	char	expstr[FT_PATH_MAX];
	char	pattern[FT_PATH_MAX];
	t_epv	epv;

	epv = (t_epv){0};
	if (token == NULL || token->str == NULL || token->str[0] == '\0')
		return (0);
	ft_memset(expstr, 0, FT_PATH_MAX);
	ft_memset(pattern, 0, FT_PATH_MAX);
	i = 0;
	while (token && token->str[i])
	{
		pattern[i] = token->str[i];
		i++;
	}
	if (expand_pattern(&epv, pattern, expstr))
		ft_strlcat(expstr, pattern, FT_PATH_MAX);
	free(token->str);
	token->str = ft_strdup(expstr);
	return (epv.count);
}
