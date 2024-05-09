/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_create_cmds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:25:01 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/09 14:28:01 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_cmd(t_cmd *cmd, t_node *node)
{
	int		i;
	int		j;
	char	**args;

	cmd->node = node;
	cmd->tokens = node->tokens;
	i = 0;
	while (node->tokens[i])
		i++;
	args = (char **)ft_calloc((i + 1), sizeof(char *));
	if (args == NULL)
		return (1);
	j = 0;
	while (j < i)
	{
		args[j] = node->tokens[j]->str;
		j++;
	}
	cmd->args = args;
	if (cmd->args == NULL)
		cmd->cmdpth = ft_strdup("");
	else if (node && node->tokens[0])
		cmd->cmdpth = ft_strdup(node->tokens[0]->str);
	return (0);
}

int	create_subshell_cmd(t_cmd *cmd, t_node *node, t_ms *ms)
{
	cmd->tokens = node->tokens;
	cmd->cmdpth = ft_strdup(ms->av[0]);
	cmd->args = (char **)ft_calloc(4, sizeof(char *));
	if (cmd->args == NULL)
		return (1);
	cmd->args[0] = ft_strdup("minishell");
	cmd->args[1] = ft_strdup("-c");
	cmd->args[2] = ft_strdup(node->tokens[0]->str);
	cmd->args[2][0] = ' ';
	cmd->args[2][ft_strlen(cmd->args[2]) - 1] = ' ';
	return (0);
}
