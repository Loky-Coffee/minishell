/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 20:24:14 by nmihaile          #+#    #+#             */
/*   Updated: 2024/05/04 18:51:31 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	has_wildcards(char *pattern)
{
	while (*pattern)
	{
		if (*pattern == '*' || *pattern == '?')
			return (0);
		pattern++;
	}
	return (1);
}

int	ft_strlchr(char *dst, const char src, size_t dstsize)
{
	size_t	d_len;
	size_t	o;

	d_len = ft_strlen(dst);
	if (d_len >= dstsize)
		return (dstsize + 1);
	o = d_len;
	dstsize = dstsize - 1;
	dst[o++] = src;
	dst[o] = 0;
	return (d_len + 1);
}

void	place_pattern(char mat[2048][2048], const char *pattern)
{
	size_t	i;

	i = 0;
	while (i < ft_strlen(pattern))
	{
		if (pattern[i] == '*')
		{
			mat[1][i + 2] = mat[1][i + 1];
			i++;
		}
		else
			mat[1][i++ + 2] = 0;
	}
}
