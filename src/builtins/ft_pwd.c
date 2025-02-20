/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:12:09 by licohen           #+#    #+#             */
/*   Updated: 2025/02/20 14:48:03 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	ft_pwd(int fd_out)
{
	char	dir[PATH_MAX];

	if (fd_out < 0)
		fd_out = STDOUT_FILENO;
	if (getcwd(dir, PATH_MAX))
	{
		ft_putendl_fd(dir, fd_out);
		return (0);
	}
	ft_putendl_fd("pwd: error retrieving current directory", 2);
	return (1);
}
