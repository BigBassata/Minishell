/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 19:24:11 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 18:44:34 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"


int	ft_env(t_environment_var *env, int fd_out)
{
	t_environment_var	*curr_env;

	curr_env = env;
	while (curr_env)
	{
		ft_putstr_fd(curr_env->key, fd_out);
		ft_putstr_fd("=", fd_out);
		ft_putendl_fd(curr_env->value, fd_out);
		curr_env = curr_env->next;
	}
	return (0);
}