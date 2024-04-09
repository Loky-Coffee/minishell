/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 23:53:16 by aalatzas          #+#    #+#             */
/*   Updated: 2024/04/09 17:27:07 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_prompt(t_ms *ms)
{
	size_t	size;
	char	cwd[1024];
	char	*last_slash;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (ft_perror("can't locate current directory."), 1); // or change msg to "getcwd"
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
	if (ms->exit_code > 0)
		ft_strlcat(ms->prompt, "\n", size);
	ft_strlcat(ms->prompt, "\033[0;92m", size);
	ft_strlcat(ms->prompt, "ninjaSHELL: /", size);
	ft_strlcat(ms->prompt, last_slash, size);
	ft_strlcat(ms->prompt, " $ \033[0m", size + 1);
	return (0);
}
