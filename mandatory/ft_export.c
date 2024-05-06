/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 18:27:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/06 20:36:41 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

int	export_arguments(t_ms *ms, t_node *node, t_token *token)
{
	t_export	export;

	if (token && token->type == TOKEN_WORD)
	{
		if (has_valid_key(0, token, export.key) == 0)
		{
			export.i = 0;
			while (ms->envp[export.i] != NULL && ft_strncmp(ms->envp[export.i], \
			token->str, keylen(token->str)) != 0)
				export.i++;
			export.operator = has_valid_operator(token->str);
			if (export.operator == -1)
				return (ft_add_unset_envvar(token->str, ms), \
				export_arguments(ms, node, token->next));
			if (ms->envp[export.i] == NULL)
				return (add_new_env_var(export, node, ms, token));
			else
				return (update_existing_env_var(export, ms, node, token));
		}
		else
			return (export_arguments(ms, node, token->next), \
			invalid_identifier(token->str));
	}
	return (0);
}

int	ft_export(t_node *node, t_ms *ms)
{
	if (node->tokens && node->tokens[0] && node->tokens[1] == NULL)
		return (ft_export_print(ms));
	expand_node(node, ms, 0);
	return (export_arguments(ms, node, node->tokens[1]));
}
