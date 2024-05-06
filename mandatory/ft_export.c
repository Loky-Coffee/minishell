/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:27:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/06 19:41:20 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	update_existing_env_var(int operator, int i, char *key, t_ms *ms, t_node *node, t_token *token);
static int	export_multi_args(t_ms *ms, t_node *node, t_token *token);

static int	has_valid_operator(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str && str[i] == '=')
			return (0);
		else if (str && str[i] == '+' && str[i + 1] == '=')
			return (i + 2);
		i++;
	}
	return (-1);
}

static int	keylen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i] && str[i] != '=' && str[i] != '+')
		i++;
	return (i);
}

static int	add_new_env_var(char *key, t_node *node, t_ms *ms, int i, t_token *token)
{
	char	**new_envp;
	int		j;

	if (token && token->str[ft_strlen(key)] == '+')
		ft_memmove(&token->str[ft_strlen(key)], &token->str[ft_strlen(key) + 1], \
		ft_strlen(&token->str[ft_strlen(key) + 1]) + 1);
	j = 0;
	new_envp = NULL;
	new_envp = ft_calloc(i + 2, sizeof(char *));
	if (!new_envp)
		return (-1);
	while (j < i)
	{
		new_envp[j] = ms->envp[j];
		j++;
	}
	new_envp[i] = ft_strdup(token->str);
	free(ms->envp);
	ms->envp = new_envp;
	ft_remove_unset_envvar(key, ms);
	return (export_multi_args(ms, node, token->next));
}

static int	has_valid_key(int i, t_token *token, char *key)
{
	if (token == NULL)
		return (1);
	ft_memset(key, 0, FT_PATH_MAX);
	while (token->str != 0 && token->str[i] && i < 4096)
	{
		if (token->str[i] == '=' || \
	(token->str[i] == '+' && token->str[i + 1] == '='))
			break ;
		ft_strlcat(key, &token->str[i], ft_strlen(key) + 2);
		i++;
	}
	if (key[0] == '_' || ft_isalpha(key[0]))
	{
		i = 1;
		while (key[i])
		{
			if (ft_isalnum(key[i]) || key[i] == '_')
				i++;
			else
				return (1);
		}
	}
	else
		return (1);
	return (0);
}

static size_t	get_envp_size(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

static int	add_unset_vars(char ***sortenv, t_ms *ms)
{
	size_t	i[3];
	char	**new_sortenv;

	if (ms->unset_envvars == NULL)
		return (0);
	i[0] = 0;
	i[1] = get_envp_size(*sortenv);
	i[2] = get_envp_size(ms->unset_envvars);
	new_sortenv = (char **)ft_calloc(i[1] + i[2] + 1, sizeof(char *));
	if (new_sortenv == NULL)
		return (1);
	while ((*sortenv)[i[0]])
	{
		new_sortenv[i[0]] = (*sortenv)[i[0]];
		i[0]++;
	}
	i[0] = 0;
	while (ms->unset_envvars[i[0]])
	{
		new_sortenv[i[1] + i[0]] = ft_strdup(ms->unset_envvars[i[0]]);
		i[0]++;
	}
	free(*sortenv);
	*sortenv = new_sortenv;
	return (0);
}

static void	sort_envp(char **envp)
{
	int		i;
	int		j;
	char	*buf;
	size_t	len;

	i = 0;
	while (envp[i + 1])
	{
		j = i + 1;
		while (envp[j])
		{
			if (ft_strlen(envp[i]) >= ft_strlen(envp[j]))
				len = ft_strlen(envp[i]) + 1;
			else
				len = ft_strlen(envp[j]) + 1;
			if (ft_strncmp(envp[i], envp[j], len) > 0)
			{
				buf = envp[i];
				envp[i] = envp[j];
				envp[j] = buf;
			}
			j++;
		}
		i++;
	}
}

static int	ft_export_print(t_ms *ms)
{
	size_t	i;
	char	**sortenv;

	i = get_envp_size(ms->envp);
	sortenv = (char **)ft_calloc(i + 1, sizeof(char *));
	if (sortenv == NULL)
		return (1);
	i = 0;
	while (ms->envp[i])
	{
		sortenv[i] = ft_strdup(ms->envp[i]);
		i++;
	}
	add_unset_vars(&sortenv, ms);
	sort_envp(sortenv);
	render_envp("declare -x ", sortenv);
	free_av(sortenv);
	return (0);
}

static void	invalid_identifier(char *str)
{
	char	errstr[FT_PATH_MAX];

	ft_memset(errstr, 0 ,FT_PATH_MAX);
	if (str && str[0] == '-')
	{
		str[2] = '\0';
		ft_strlcat(errstr, str, FT_PATH_MAX);
		ft_error("export", errstr, "invalid option");
	}
	else
	{
		ft_strlcat(errstr, "`", FT_PATH_MAX);
		ft_strlcat(errstr, str, FT_PATH_MAX);
		ft_strlcat(errstr, "'", FT_PATH_MAX);
		ft_error("export", errstr, "not a valid identifier");
	}

}

static int	update_existing_env_var(int operator, int i, char *key, t_ms *ms, t_node *node, t_token *token)
{
	char	*newstr;
	size_t	len1;
	size_t	len2;
	char	*buf;

	if (operator == 0)
	{
		buf = ft_strdup(token->str);
		free(ms->envp[i]);
		ms->envp[i] = buf;
	}
	else if (operator > 0)
	{
		buf = ft_strdup(token->str);
		if (!(buf == NULL || buf[0] == '\0'))
		{
			len1 = ft_strlen(ms->envp[i]);
			len2 = ft_strlen(&buf[operator]);
			newstr = (char *)ft_calloc(len1 + len2 + 1, sizeof(char));
			if (newstr == NULL)
				return (1);
			ft_strlcat(newstr, ms->envp[i], len1 + len2 + 1);
			ft_strlcat(newstr, &buf[operator], len1 + len2 + 1);
			free(ms->envp[i]);
			free(buf);
			ms->envp[i] = newstr;
		}
	}
	ft_remove_unset_envvar(key, ms);
	return (export_multi_args(ms, node, token->next));
}

static int	export_multi_args(t_ms *ms, t_node *node, t_token *token)
{
	char	key[FT_PATH_MAX];
	int		operator;
	int		i;

	if (token && token->type == TOKEN_WORD)
	{
		if (has_valid_key(0, token, key) == 0)
		{
			i = 0;
			while (ms->envp[i] != NULL && ft_strncmp(ms->envp[i], token->str, keylen(token->str)) != 0)
				i++;
			operator = has_valid_operator(token->str);
			if (operator == -1)
			{
				ft_add_unset_envvar(token->str, ms);
				export_multi_args(ms, node, token->next);
				return (0);
			}
			if (ms->envp[i] == NULL)
				return (add_new_env_var(key, node, ms, i, token));
			else
				return (update_existing_env_var(operator, i, key, ms, node, token));
		}
		else
		{
			invalid_identifier(token->str);
			export_multi_args(ms, node, token->next);
			return (1);
		}
	}
	return (0);
}

int	ft_export(t_node *node, t_ms *ms)
{
	if (node->tokens && node->tokens[0] && node->tokens[1] == NULL)
		return (ft_export_print(ms));
	expand_node(node, ms, 0);
	return (export_multi_args(ms, node, node->tokens[1]));
}
