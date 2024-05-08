/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_redirects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:46:43 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/08 15:47:47 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	redirect_in(int *fd_in, t_node *node, t_ms *ms)
{
	char	var_buf[FT_PATH_MAX];

	ft_strlcpy(var_buf, node->tokens[1]->str, FT_PATH_MAX);
	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	if ((node->tokens[1]->str[0] == '\0' || node->tokens[2] != NULL) \
	&& ft_strchr(var_buf, '"') == NULL)
		return (ft_error(var_buf, "ambiguous redirect", NULL), 1);
	close(*fd_in);
	*fd_in = open(node->tokens[1]->str, O_RDONLY, 0644);
	if (*fd_in < 0)
		return (ft_error(node->tokens[1]->str, \
		"No such file or directory", NULL), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static int	redirect_out(int *fd_out, t_node *node, t_ms *ms)
{
	char	var_buf[FT_PATH_MAX];

	ft_strlcpy(var_buf, node->tokens[1]->str, FT_PATH_MAX);
	if (expand_node(node, ms))
		return (EXIT_FAILURE);
	if ((node->tokens[1]->str[0] == '\0' || node->tokens[2] != NULL) \
	&& ft_strchr(var_buf, '"') == NULL)
		return (ft_error(var_buf, "ambiguous redirect", NULL), EXIT_FAILURE);
	close(*fd_out);
	if (node->tokens[0]->type == TOKEN_DGREATER)
		*fd_out = open(node->tokens[1]->str, \
		O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd_out = open(node->tokens[1]->str, \
		O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd_out < 0)
		return (ft_error(node->tokens[1]->str, \
		strerror(errno), NULL), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

pid_t	redirect_herestring(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;

	pid = EXIT_SUCCESS;
	expand_node(node, ms);
	execute_herestring(&fd_in, &fd_out, node->tokens[1]->str, ms);
	if (node->left && (node->left->type == NODE_REDIRECT \
	|| node->left->type == NODE_COMMAND || node->left->type == NODE_SUBSHELL))
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	else
		close(fd_in);
	return (pid);
}

static pid_t	redirect_heredoc(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;

	pid = EXIT_SUCCESS;
	execute_heredoc(&fd_in, &fd_out, node->tokens[1]->str, ms);
	if (ms->hd_interupt == 0 && node->left \
	&& (node->left->type == NODE_REDIRECT \
	|| node->left->type == NODE_COMMAND || node->left->type == NODE_SUBSHELL))
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	else
		close(fd_in);
	return (pid);
}

pid_t	redirect_manager(int fd_in, int fd_out, t_node *node, t_ms *ms)
{
	pid_t	pid;

	pid = EXIT_SUCCESS;
	if (node->tokens == NULL)
		return (EXIT_FAILURE);
	if (node->tokens[0]->type == TOKEN_TLESS)
		pid = redirect_herestring(fd_in, fd_out, node, ms);
	else if (node->tokens[0]->type == TOKEN_DLESS)
		pid = redirect_heredoc(fd_in, fd_out, node, ms);
	else if (node->tokens[0]->type == TOKEN_LESS && node->tokens[1])
	{
		if (redirect_in(&fd_in, node, ms) > 0)
			return (1);
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	else if (node->tokens[0]->type == TOKEN_GREATER \
	|| node->tokens[0]->type == TOKEN_DGREATER)
	{
		if (redirect_out(&fd_out, node, ms) > 0)
			return (1);
		pid = exec_intermediary(fd_in, fd_out, node->left, ms);
	}
	return (pid);
}
