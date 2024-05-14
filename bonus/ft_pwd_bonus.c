/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 21:42:17 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/14 21:42:18 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell_bonus.h"

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
