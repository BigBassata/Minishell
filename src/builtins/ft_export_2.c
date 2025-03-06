/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:14:54 by licohen           #+#    #+#             */
/*   Updated: 2025/02/20 14:47:57 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int	args_counter(char **args)
{
	int	i;

	if (!args || !args[0])
		return (0);
	i = 0;
	while (args[i])
		i++;
	return (i);
}

int	ft_export_loop(char	**args, t_environment_var *env, int fd_out)
{
	int	args_nb;
	int	i;

	args_nb = args_counter(args);
	if (args_nb < 2)
		return (ft_export(args[1], env, fd_out));
	else
	{
		i = 0;
		while (i < args_nb)
		{
			if (i == args_nb - 1)
				return (ft_export(args[i], env, fd_out));
			if (ft_export(args[i], env, fd_out) != 0)
				return (1);
			i++;
		}
		return (0);
	}
}
