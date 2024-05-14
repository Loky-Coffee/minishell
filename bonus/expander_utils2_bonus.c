/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:41:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:41:32 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

void	expand_single_char(char *dst, int *j, char *src, int *i)
{
	(*i)++;
	if (src[(*i)])
		dst[(*j)++] = src[(*i)];
}

void	expand_quote(char *qm, char *dst, int *j, char *src)
{
	if (*qm == '\0')
		*qm = *src;
	else if (*qm == *src)
		*qm = '\0';
	else
		dst[(*j)++] = *src;
}

void	set_zero(int *a, int *flag)
{
	*a = 0;
	*flag = 0;
}

int	process_braces_token_read_line(char **a)
{
	while (1)
	{
		*a = readline("> ");
		if (*a == NULL)
			return (ft_syntax_error("unexpected EOF while \
			looking for matching", "}", NULL), 1);
		if (ft_strchr(*a, '}') != 0)
			break ;
	}
	return (0);
}
