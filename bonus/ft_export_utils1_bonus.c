/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils1_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:42:05 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:42:07 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	invalid_identifier(char *str)
{
	char	errstr[FT_PATH_MAX];

	ft_memset(errstr, 0, FT_PATH_MAX);
	if (str && str[0] == '-')
	{
		str[2] = '\0';
		ft_strlcat(errstr, str, FT_PATH_MAX);
		ft_error("export", errstr, "invalid option");
		return (2);
	}
	ft_strlcat(errstr, "`", FT_PATH_MAX);
	ft_strlcat(errstr, str, FT_PATH_MAX);
	ft_strlcat(errstr, "'", FT_PATH_MAX);
	ft_error("export", errstr, "not a valid identifier");
	return (1);
}

static int	append_to_existing_envvar(t_export export, char *buf, t_ms *ms)
{
	size_t	len[2];
	char	*newstr;

	len[0] = ft_strlen(ms->envp[export.i]);
	len[1] = ft_strlen(&buf[export.operator]);
	newstr = (char *)ft_calloc(len[0] + len[1] + 1, sizeof(char));
	if (newstr == NULL)
		return (1);
	ft_strlcat(newstr, ms->envp[export.i], len[0] + len[1] + 1);
	ft_strlcat(newstr, &buf[export.operator], len[0] + len[1] + 1);
	free(ms->envp[export.i]);
	free(buf);
	ms->envp[export.i] = newstr;
	return (0);
}

int	update_existing_env_var(t_export export, t_ms *ms,
				t_node *node, t_token *token)
{
	char	*buf;

	if (export.operator == 0)
	{
		buf = ft_strdup(token->str);
		free(ms->envp[export.i]);
		ms->envp[export.i] = buf;
	}
	else if (export.operator > 0)
	{
		buf = ft_strdup(token->str);
		if (!(buf == NULL || buf[0] == '\0'))
		{
			if (append_to_existing_envvar(export, buf, ms))
				return (1);
		}
	}
	ft_remove_unset_envvar(export.key, ms);
	return (export_arguments(ms, node, token->next));
}
