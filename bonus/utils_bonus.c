/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:44:17 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:44:19 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	free_line(t_ms *ms)
{
	if (ms->line)
	{
		free(ms->line);
		ms->line = NULL;
	}
}

void	free_cmd(t_cmd *cmd)
{
	cmd->tokens = NULL;
	if (cmd->cmdpth)
		free(cmd->cmdpth);
	cmd->cmdpth = NULL;
	if (cmd->args)
		free(cmd->args);
	cmd->args = NULL;
}

void	cleanup_ms(t_ms *ms)
{
	if (ms->line)
		free_line(ms);
	ms->line = NULL;
	ft_token_clear(&ms->tokens, del_token_content);
	ms->tokens = NULL;
	free_nodetree(&ms->nodes);
	ms->parse_error = 0;
	ms->parse_errtkn = NULL;
	ms->hd_interupt = 0;
}
