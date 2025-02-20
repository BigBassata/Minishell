/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 19:24:11 by licohen           #+#    #+#             */
/*   Updated: 2025/02/20 14:47:43 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int ft_env(t_environment_var *env, int fd_out)
{
	if (fd_out < 0)
		fd_out = STDOUT_FILENO;
	if (!env)
		return (0);
	if (env && ft_strcmp(env->key, "_?") == 0)
		env = env->next;
	while (env)
	{
		if (env->value && *env->value)
		{
			ft_putstr_fd(env->key, fd_out);
			ft_putchar_fd('=', fd_out);
			ft_putendl_fd(env->value, fd_out);
		}
		env = env->next;
	}
	return (0);
}
