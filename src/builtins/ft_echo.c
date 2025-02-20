/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:43:45 by licohen           #+#    #+#             */
/*   Updated: 2025/02/20 14:47:36 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int	is_valid_n_option(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	if (str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static int	handle_no_args(int fd_out)
{
	ft_putchar_fd('\n', fd_out);
	return (0);
}

static void	print_args(char **array, int start_index, int fd_out)
{
	int		i;
	char	*arg;

	i = start_index;
	while (array[i])
	{
		arg = array[i];
		if (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"')
		{
			arg++;
			arg[ft_strlen(arg) - 1] = '\0';
		}
		ft_putstr_fd(arg, fd_out);
		if (array[i + 1])
			ft_putchar_fd(' ', fd_out);
		i++;
	}
}

int	ft_echo(char **array, int fd_out)
{
	int	i;
	int	has_n_option;

	if (!array)
		return (1);
	if (nbr_of_args(array) < 2)
		return (handle_no_args(fd_out));
	if (fd_out < 0)
		fd_out = STDOUT_FILENO;
	has_n_option = 0;
	i = 1;
	while (array[i] && is_valid_n_option(array[i]))
	{
		has_n_option = 1;
		i++;
	}
	print_args(array, i, fd_out);
	if (!has_n_option)
		ft_putchar_fd('\n', fd_out);
	return (0);
}
