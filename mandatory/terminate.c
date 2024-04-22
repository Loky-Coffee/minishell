/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 18:48:38 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/22 18:32:52 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	del_token_content(void *param)
{
	char	*str;

	str = (char *)param;
	if (str)
		free(str);
}

void	free_ms(t_ms *ms)
{
	ft_token_clear(&ms->tokens, del_token_content);

	// Free more if we hve more
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
	if (fdr > 2)
		close(fdr);
	if (fdw > 2)
		close(fdw);
	// if (fdr != STDIN_FILENO && fdr != STDOUT_FILENO)
	// 	close(fdr);
	// if (fdw != STDOUT_FILENO && fdw != STDIN_FILENO)
	// 	close(fdw);
}

// void	ft_force_close_fd(int fdr, int fdw)
// {
// 	if (fdr >= 0)
// 		close(fdr);
// 	if (fdw >= 0)
// 		close(fdw);
// }

void	free_nodetree(t_node **n)
{
	if (*n == NULL)
		return ;
	if ((*n)->left)
		free_nodetree(&(*n)->left);
	if ((*n)->right)
		free_nodetree(&(*n)->right);
	free(*n);
	*n = NULL;
}

void	terminate(t_ms *ms, t_cmd *cmd, int exit_code)
{
	rl_clear_history();
	if (cmd)
		free_cmd(cmd);
	free_line(ms);
	if (ms->tokens)
		ft_token_clear(&ms->tokens, del_token_content);
	ms->tokens = NULL;
	free_nodetree(&ms->nodes);
	ft_token_clear(&ms->tokens, del_token_content);
	if (ms->unset_envvars)
		free_av(ms->unset_envvars);
	exit(exit_code);
}
