/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 18:16:23 by nmihaile          #+#    #+#             */
/*   Updated: 2024/04/13 18:15:21 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// PWD returns the curren path using the getcwd() function
// ==> it does not return the value of the PWD= envoronment variable !!!! <==
int	ft_pwd(void)
{
	char	pwd[FT_PATH_MAX];

	ft_memset(pwd, 0, FT_PATH_MAX);
	getcwd(pwd, FT_PATH_MAX);
	if (pwd[0])
	{
		printf("%s\n", pwd);
		return (0);
	}
	return (1);
}
