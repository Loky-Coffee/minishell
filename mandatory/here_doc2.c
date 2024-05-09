/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 17:45:36 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/09 17:54:54 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	hd_process_braced_envvar(char **line, int *i, t_ms *ms, char *expstr)
{
	if (ft_strchr(&(*line)[*i], '}'))
	{
		(*i)++;
		set_key(&(*line)[*i], ms->key, ms);
		ft_get_env_value(ms, ms->value, ms->key);
		ft_strlcat(expstr, ms->value, FT_PATH_MAX);
		while ((*line)[*i] != '}')
			(*i)++;
	}
	else
		return (1);
	return (0);
}

static int	expand_hd_parameter_and_variable(char **line, char *expstr, \
int *i, t_ms *ms)
{
	char	*exit_str;

	(*i)++;
	if ((*line)[*i] == '?')
	{
		exit_str = ft_itoa(ms->exit_code);
		ft_strlcat(expstr, exit_str, FT_PATH_MAX);
		return (free(exit_str), 0);
	}
	else if ((*line)[*i] == '0')
	{
		exit_str = ft_strdup(NINJASHELL);
		ft_strlcat(expstr, exit_str, FT_PATH_MAX);
		return (free(exit_str), 0);
	}
	else if ((*line)[*i] == '{')
		return (hd_process_braced_envvar(line, i, ms, expstr));
	set_key(&(*line)[*i], ms->key, ms);
	ft_get_env_value(ms, ms->value, ms->key);
	ft_strlcat(expstr, ms->value, FT_PATH_MAX);
	(*i) = (*i) + ft_strlen(ms->key) - 1;
	return (0);
}

static int	realloc_line(char **line, char *expstr)
{
	int	i;
	
	free(*line);
	*line = ft_calloc(ft_strlen(expstr) + 1, sizeof(char));
	if (*line == NULL)
		return (1);
	i = 0;
	while (expstr[i])
	{
		(*line)[i] = expstr[i];
		i++;
	}
	return (0);
}

int	expand_hd_line(char **line, t_ms *ms, int i, int j)
{
	char	expstr[FT_PATH_MAX];
	int		err;

	err = 0;
	ft_memset(expstr, 0, FT_PATH_MAX);
	while (*line && i < (int)ft_strlen(*line) && (*line)[i])
	{
		if ((*line)[i] == '$' && (*line)[i + 1] != 0 \
		&& (ft_isalnum((*line)[i + 1]) || (*line)[i + 1] == '_' \
		|| (*line)[i + 1] == '{'))
		{
			err = expand_hd_parameter_and_variable(line, expstr, &i, ms);
			while (expstr[j])
				j++;
		}
		else
			expstr[j++] = (*line)[i];
		i++;
	}
	if (realloc_line(line, expstr))
		err = -1;
	return (err);
}
