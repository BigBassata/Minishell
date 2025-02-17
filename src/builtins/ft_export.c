/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 18:14:54 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 18:44:42 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int	is_valid_exported_env(char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(*name))
		return (ERROR);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (ERROR);
	i = 1;
	while (name[i] && name[i] != '=')
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (ERROR);
		i++;
	}
	return (TRUE);
}

static void	ft_export_without_args(t_environment_var *env, int fd_out)
{
	t_environment_var	*curr_env;

	curr_env = env;
	// si comme dans bash _? ne doit pas être affiché
	if (curr_env && ft_strcmp(curr_env->key, "_?") == 0)
		curr_env = curr_env->next;
	while (curr_env)
	{
		ft_putstr_fd("declare -x ", fd_out);
		ft_putstr_fd(curr_env->key, fd_out);
		ft_putstr_fd("=\"", fd_out);
		ft_putstr_fd(curr_env->value, fd_out);
		ft_putendl_fd("\"", fd_out);
		curr_env = curr_env->next;
	}
}

static int	handle_split_env_var(char **name, char **value, char *new_var)
{
	int	name_len;
	int	value_len;

	name_len = 0;
	while (new_var[name_len] != '=')
		name_len++;
	*name = (char *)malloc((name_len + 1) * sizeof(char));
	if (!*name)
		return (print_error_message("error malloc in export"), ERROR);
	ft_strlcpy(*name, new_var, name_len + 1);
	if (ft_strcmp(*name, "PWD") == 0 || ft_strcmp(*name, "OLDPWD") == 0)
		return (free(*name), print_error_exec_message(READONLY_VARIABLE,
				new_var), ERROR);
	name_len++;
	value_len = 0;
	while (new_var[name_len + value_len] != '\0')
		value_len++;
	*value = (char *)malloc((value_len + 1) * sizeof(char));
	if (!*value)
		return (free(*name), print_error_message("error malloc export"),
			ERROR);
	ft_strlcpy(*value, &new_var[name_len], value_len + 1);
	return (TRUE);
}

static void	replace_env_var_value(t_environment_var *env, char *value)
{
	free(env->value);
	env->value = value;
}

int	ft_export(char *arg_1, t_environment_var *env, int fd_out)
{
	char				*name;
	char				*value;
	t_environment_var	*curr_env;

	name = NULL;
	value = NULL;
	if (!arg_1)
		return (ft_export_without_args(env, fd_out), 0);
	if (is_valid_exported_env(arg_1) == ERROR)
		return (print_error_exec_message(NOT_A_VALID_IDENTIFIER, arg_1),
			1);
	if (!ft_strchr(arg_1, '='))
		return (1);
	if (handle_split_env_var(&name, &value, arg_1) == ERROR)
		return (1);
	curr_env = env;
	while (curr_env)
	{
		if (ft_strcmp(name, curr_env->key) == 0)
			return (replace_env_var_value(curr_env, value), 0);
		curr_env = curr_env->next;
	}
	if (create_env_var(&env, name, value) == ERROR)
		return (print_error_message("error create env"), 1);
	return (0);
}
