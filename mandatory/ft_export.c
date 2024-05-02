/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:27:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/02 17:02:14 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
	while (str[i] != '=' && str[i] != '+')
		i++;
	return (i);
}

static char	*tkn_to_str(t_token *token, t_ms *ms)
{
	expand_tkn(token, ms);
	return (ft_strdup(token->str));
}

static int	add_new_env_var(char *key, t_ms *ms, int i)
{
	char	**new_envp;
	int		j;

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
	new_envp[i] = tkn_to_str(ms->tokens->next, ms);
	free(ms->envp);
	ms->envp = new_envp;
	ft_remove_unset_envvar(key, ms);
	return (0);
}

static int	has_valid_key(int i, t_ms *ms, char *key)
{
	if (ms->tokens->next == NULL)
		return (1);
	ft_memset(key, 0, FT_PATH_MAX);
	while (ms->tokens->next->str != 0 && ms->tokens->next->str[i] && i < 4096)
	{
		if (ms->tokens->next->str[i] == '=' || ms->tokens->next->str[i] == '+')
			break ;
		ft_strlcat(key, &ms->tokens->next->str[i], ft_strlen(key) + 2);
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

static int	update_existing_env_var(int operator, int i, char *key, t_ms *ms)
{
	char	*newstr;
	size_t	len1;
	size_t	len2;

	if (operator == 0)
	{
		free(ms->envp[i]);
		ms->envp[i] = tkn_to_str(ms->tokens->next, ms);
	}
	else if (operator > 0)
	{
		len1 = ft_strlen(ms->envp[i]);
		len2 = ft_strlen(&ms->nodes->tokens[1]->str[operator]);
		newstr = (char *)ft_calloc(len1 + len2 + 1, sizeof(char));
		if (newstr == NULL)
			return (1);
		ft_strlcat(newstr, ms->envp[i], len1 + len2 + 1);
		ft_strlcat(newstr, &ms->nodes->tokens[1]->str[operator], \
		len1 + len2 + 1);
		free(ms->envp[i]);
		ms->envp[i] = newstr;
	}
	ft_remove_unset_envvar(key, ms);
	return (0);
}

int	ft_export(int i, t_node *node, t_ms *ms)
{
	char	key[FT_PATH_MAX];
	int		operator;

	if (node->tokens && node->tokens[0] && node->tokens[1] == NULL)
		return (ft_export_print(ms));
	if (has_valid_key(0, ms, key) == 1)
	{
		if (ms->tokens->next && ms->tokens->next->str[0] == '-')
			return (ft_error("export", ms->tokens->next->str, \
			"invalid option"), 2);
		else if (ms->tokens->next)
			return (ft_error("export", ms->tokens->next->str, \
			"not a valid identifier"), 1);
		else
			return (printf(LIGHTCYAN"export: Provide a valid key"RESET), 1);
	}
	operator = has_valid_operator(ms->nodes->tokens[1]->str);
	if (operator == -1)
		return (ft_add_unset_envvar(ms->tokens->next->str, ms), 0);
	while (ms->envp[i] != NULL && ft_strncmp(ms->envp[i], \
	ms->nodes->tokens[1]->str, keylen(ms->nodes->tokens[1]->str)) != 0)
		i++;
	if (ms->envp[i] == NULL)
		add_new_env_var(key, ms, i);
	else
		update_existing_env_var(operator, i, key, ms);
	return (0);
}
