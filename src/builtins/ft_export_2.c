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
		i = 1;
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

int	handle_var_without_equal_sign(char *arg_1, t_environment_var *env)
{
	t_environment_var	*curr_env;
	
	curr_env = env;
	while (curr_env)
	{
		if (ft_strcmp(arg_1, curr_env->key) == 0)
			return (0);
		curr_env = curr_env->next;
	}
	if (create_env_var(&env, arg_1, "\0") == ERROR)
		return (print_error_message("error create env var"), 1);
	return (0);
}

int	handle_void_env_var_value(char **name, char **value)
{
	*value = (char *)malloc(3 * sizeof(char));
	if (!*value)
		return (free(*name), print_error_message("error malloc export"),
			ERROR);
	ft_strlcpy(*value, "\"\"", 3);
	return (TRUE);
}
