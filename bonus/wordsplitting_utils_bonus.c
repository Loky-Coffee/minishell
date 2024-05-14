/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordsplitting_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:44:49 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:44:50 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

int	has_space(char *str)
{
	while (*str)
	{
		if (ft_isspace(*str))
			return (1);
		str++;
	}
	return (0);
}

char	*ft_skipspace(char *str, size_t *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	return (&str[*i]);
}

int	ft_get_word_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && !ft_isspace(str[len]))
		len++;
	return (len);
}
