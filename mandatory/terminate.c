/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:48:38 by nmihaile          #+#    #+#             */
/*   Updated: 2024/03/28 16:50:03 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* ************************************************************************** */
// del_token safely frees a single token, ensuring it exists before freeing.
// It's designed for use with token list cleanup functions.
void	del_token_content(void *param)
{
	char	*str;

	str = (char *)param;
	if (str)
		free(str);
}

/* ************************************************************************** */
// free_ms initiates the cleanup of the entire ms structure, specifically
// clearing the token list using ft_token_clear and del_token for each token.
void	free_ms(t_ms *ms)
{
	ft_token_clear(&ms->tokens, del_token_content);
	
	// free all
}

void	free_av(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		free(av[i++]);
	free(av);
}

void	ft_close_fd(int fdr, int fdw)
{
	if (fdr != STDIN_FILENO && fdr != STDOUT_FILENO)
		close(fdr);
	if (fdw != STDOUT_FILENO && fdw != STDIN_FILENO)
		close(fdw);
}

void	free_nodetree(t_node **n)
{
	if (*n == NULL)
		return ;
	// if ((*n)->tokens)
	// 	free((*n)->tokens);
	// (*n)->tokens = NULL;
	if ((*n)->left)
		free_nodetree(&(*n)->left);
	if ((*n)->right)
		free_nodetree(&(*n)->right);
	free(*n);
	*n = NULL;
}

void	terminate(t_ms *ms, t_cmd *cmd, int exit_code)
{
	clear_history();
	if (cmd)
		free_cmd(cmd);
	// {
	// 	free(cmd->cmdpth);
	// 	free_av(cmd->args);		
	// }
	free_line(ms);
	if (ms->tokens)
		ft_token_clear(&ms->tokens, del_token_content);
	ms->tokens = NULL;
	free_nodetree(&ms->nodes);
	free_ms(ms);
	exit(exit_code);
}
