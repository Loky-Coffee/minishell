/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:27:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/22 21:51:12 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	has_equal_sign(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str && str[i] == '=')
			return (0);
		i++;
	}
	return (1);
}

static int	keylen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
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

static int	has_valid_key(t_ms *ms, char *key)
{
	int	i;

	if (ms->tokens->next == NULL)
		return (1);
	i = 0;
	ft_memset(key, 0, FT_PATH_MAX);
	while (ms->tokens->next->str != NULL && ms->tokens->next->str[i] && i < FT_PATH_MAX)
	{
		if (ms->tokens->next->str[i] == '=')
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
	size_t	i;
	size_t	len1;
	size_t	len2;
	char	**new_sortenv;

	if (ms->unset_envvars == NULL)
		return (0);
	i = 0;
	len1 = get_envp_size(*sortenv);
	len2 = get_envp_size(ms->unset_envvars);
	new_sortenv = (char **)ft_calloc(len1 + len2 + 1, sizeof(char *));
	if (new_sortenv == NULL)
		return (1);
	while ((*sortenv)[i])
	{
		new_sortenv[i] = (*sortenv)[i];
		i++;
	}
	i = 0;
	while (ms->unset_envvars[i])
	{
		new_sortenv[len1 + i] = ft_strdup(ms->unset_envvars[i]);
		i++;
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

int	ft_export(t_node *node, t_ms *ms)
{
	int		i;
	char	key[FT_PATH_MAX];

	if (node->tokens && node->tokens[0] && node->tokens[1] == NULL)
		return (ft_export_print(ms));
	if (has_valid_key(ms, key) == 1)
	{
		if (ms->tokens->next)
			return (ft_error("export", ms->tokens->next->str, \
			"not a valid identifier"), 1);
		else
			return (printf(LIGHTCYAN"export: Provide a valid key"RESET, NULL), 1);
	}
	if (ms->tokens->next && has_equal_sign(ms->tokens->next->str))
		return (ft_add_unset_envvar(ms->tokens->next->str, ms), 0);
	i = 0;
	while (ms->envp[i] != NULL && ft_strncmp(ms->envp[i], \
	ms->tokens->next->str, keylen(ms->tokens->next->str)) != 0)
		i++;
	if (ms->envp[i] == NULL)
		add_new_env_var(key, ms, i);
	else
	{
		free(ms->envp[i]);
		ms->envp[i] = tkn_to_str(ms->tokens->next, ms);
		ft_remove_unset_envvar(key, ms);
	}
	return (0);
}
