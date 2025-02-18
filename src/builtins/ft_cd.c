/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:43:42 by licohen           #+#    #+#             */
/*   Updated: 2025/02/13 14:48:34 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "minishell_exec.h"

static int	update_env_var_value(char *key, char *value, t_environment_var *env)
{
	t_environment_var	*current;
	char				*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return (print_error_message("cd: error malloc new env value"), ERROR);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (current->value)
				free(current->value);
			current->value = new_value;
			return (TRUE);
		}
		current = current->next;
	}
	return (free(new_value), ERROR);
}

static int	is_env_var_exist(char *key, t_environment_var *env)
{
	t_environment_var	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (TRUE);
		current = current->next;
	}
	return (FALSE);
}

int	update_or_create_pwd(t_environment_var *env)
{
	char	*current_dir;

	current_dir = ft_calloc(PATH_MAX, 1);
	if (!current_dir)
		return (print_error_message("cd: malloc error create PWD var"), ERROR);
	if (!getcwd(current_dir, PATH_MAX))
		return (free(current_dir),
			print_error_exec_message(CD_ERR_RETRIEVING_CURR_DIRECTORY,
				"cd"), ERROR);
	if (!is_env_var_exist("PWD", env))
	{
		if (create_env_var_in_cd(&env, "PWD", current_dir) == ERROR)
			return (free(current_dir), 
				print_error_message("cd: error create PWD var"), ERROR);
	}
	else
	{
		if (update_env_var_value("PWD", current_dir, env) == ERROR)
			return (free(current_dir), 
				print_error_message("cd: error update PWD var"), ERROR);
	}
	free(current_dir);
	return (TRUE);
}

int	update_or_create_oldpwd(t_environment_var *env)
{
	t_environment_var	*pwd;

	pwd = env;
	while (pwd)
	{
		if (ft_strcmp(pwd->key, "PWD") == 0)
			break ;
		pwd = pwd->next;
	}
	if (!pwd)
		return (print_error_message("cd: no PWD env var after chdir"), ERROR);
	if (!is_env_var_exist("OLDPWD", env))
	{
		if (create_env_var_in_cd(&env, "OLDPWD", pwd->value) == ERROR)
			return (print_error_message("cd: error create OLDPWD var"), ERROR);
	}
	else
	{
		if (update_env_var_value("OLDPWD", pwd->value, env) == ERROR)
			return (print_error_message("cd: error update OLDPWD var"), ERROR);
	}
	return (TRUE);
}

// int	ft_cd(char **args, t_environment_var *env)
// {
// 	int chdir_result;

// 	if (nbr_of_args(args) == 1 || !args[1] || !*args[1])
// 		return (print_error_exec_message(CD_NEED_RELATIVE_OR_ABSOLUTE_PATH,
// 				"cd"), 1);
// 	if (nbr_of_args(args) > 2)
// 		return (print_error_exec_message(TOO_MANY_ARGUMENTS, "cd"), 1);
// 	if (args[1][0] == '-' && args[1][1] == '\0')
// 		return (cd_to_oldpwd(env));
// 	if (args[1][0] == '~')
// 		return (cd_with_tild(args, env));
// 	if (update_or_create_pwd(env) == ERROR)
// 		return (1);
// 	chdir_result = chdir(args[1]);
// 	if (chdir_result == -1)
// 		return (handle_cd_errors(args[1]), 1);
// 	if (update_or_create_oldpwd(env) == ERROR)
// 		return (1);
// 	if (update_or_create_pwd(env) == ERROR)
// 		return (1);
// 	return (0);
// }

int ft_cd(char **args, t_environment_var *env)
{
	char *target_path;
	
	// cd sans arguments -> aller au HOME
	// ***Attention*** sujet demande: cd with only a relative or absolute path
	if (nbr_of_args(args) == 1 || !args[1] || !*args[1])
	{
		target_path = get_env_value(env, "HOME");
		if (!target_path)
			return (print_error_exec_message(HOME_NOT_SET, "cd"), 1);
	}
	else if (nbr_of_args(args) > 2)
		return (print_error_exec_message(TOO_MANY_ARGUMENTS, "cd"), 1);
	else if (args[1][0] == '-' && args[1][1] == '\0')
		return (cd_to_oldpwd(env));
	else if (args[1][0] == '~')
		return (cd_with_tild(args, env));
	else
		target_path = args[1];

	// ***Attention*** bien s'assurer que PWD existe ici, sinon update_or_create_oldpwd() return ERROR
	if (update_or_create_pwd(env) == ERROR)
		return (1);

	// Changer de répertoire
	if (chdir(target_path) == -1)
		return (handle_cd_errors(target_path), 1);

	// Sauvegarder l'ancien PWD avant de le mettre à jour
	if (update_or_create_oldpwd(env) == ERROR)
		return (1);

	// Mettre à jour PWD après le changement
	if (update_or_create_pwd(env) == ERROR)
		return (1);

	return (0);
}
