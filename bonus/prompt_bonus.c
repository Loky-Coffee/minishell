/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:43:35 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:43:36 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	create_prompt(t_ms *ms)
{
	size_t	size;
	char	cwd[1024];
	char	*last_slash;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (ft_perror("prompt directory"), 1);
	last_slash = ft_strrchr(cwd, '/');
	if (last_slash != NULL)
		last_slash++;
	else
		last_slash = cwd;
	if (ms->prompt)
		free(ms->prompt);
	size = 9 + 13 + ft_strlen(last_slash) + 9 + 1;
	ms->prompt = (char *)ft_calloc(size, sizeof(char));
	if (ms->prompt == NULL)
		return (1);
	ft_strlcat(ms->prompt, LIGHTGREEN, size);
	ft_strlcat(ms->prompt, NINJASHELL, size);
	ft_strlcat(ms->prompt, ": /", size);
	ft_strlcat(ms->prompt, last_slash, size);
	ft_strlcat(ms->prompt, " $ ", size + 1);
	ft_strlcat(ms->prompt, RESET, size + 1);
	return (0);
}
