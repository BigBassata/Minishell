/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:43:42 by licohen           #+#    #+#             */
/*   Updated: 2025/02/06 18:33:26 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "minishell_exec.h"

static t_environment_var	*get_env_var_by_key(char *key,
	t_environment_var *env)
{
	t_environment_var	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	cd_to_oldpwd(t_environment_var *env)
{
	char				path[PATH_MAX];
	t_environment_var	*oldpwd;

	ft_memset(path, 0, PATH_MAX);
	if (update_or_create_pwd(env) == ERROR)
		return (1);
	oldpwd = get_env_var_by_key("OLDPWD", env);
	if (!oldpwd || !oldpwd->value)
		return (print_error_exec_message(OLDPWD_NOT_SET, "cd"), 1);
	ft_strlcpy(path, oldpwd->value, PATH_MAX);
	if (chdir(path) == -1)
		return (handle_cd_errors(path), 1);
	if (update_or_create_oldpwd(env) == ERROR)
		return (1);
	if (update_or_create_pwd(env) == ERROR)
		return (1);
	return (0);
}

int	cd_with_tild(char **args, t_environment_var *env)
{
	char				path[PATH_MAX];
	t_environment_var	*home;

	ft_memset(path, 0, PATH_MAX);
	if (update_or_create_pwd(env) == ERROR)
		return (1);
	home = get_env_var_by_key("HOME", env);
	if (!home || !home->value)
		return (print_error_exec_message(HOME_NOT_SET, "cd"), 1);
	ft_strlcpy(path, home->value, PATH_MAX);
	ft_strlcat(path, &args[1][1], PATH_MAX);
	if (chdir(path) == -1)
		return (handle_cd_errors(path), 1);
	if (update_or_create_oldpwd(env) == ERROR)
		return (1);
	if (update_or_create_pwd(env) == ERROR)
		return (1);
	return (0);
}
