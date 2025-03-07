/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:09:46 by licohen           #+#    #+#             */
/*   Updated: 2025/02/25 16:05:20 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	initialize_env_node(t_environment_var *node)
{
	node->ctr_d_in_heredoc = 0;
	node->last_exit_code = 0;
	node->next = NULL;
}

static int	parse_env_string(t_environment_var *node, char *envp,
	char *equals_sign)
{
	if (equals_sign)
	{
		node->key = ft_substr(envp, 0, equals_sign - envp);
		node->value = ft_strdup(equals_sign + 1);
	}
	else
	{
		node->key = ft_strdup(envp);
		node->value = ft_strdup("");
	}
	if (!node->key || !node->value)
	{
		cleanup_ptr(node->key);
		cleanup_ptr(node->value);
		cleanup_ptr(node);
		return (0);
	}
	return (1);
}

t_environment_var	*create_environment_node(char *envp)
{
	t_environment_var	*new_node;
	char				*equals_sign;

	new_node = malloc(sizeof(t_environment_var));
	if (!new_node)
		return (NULL);
	initialize_env_node(new_node);
	equals_sign = ft_strchr(envp, '=');
	if (!parse_env_string(new_node, envp, equals_sign))
		return (NULL);
	return (new_node);
}

t_environment_var	*init_environment(char **envp)
{
	t_environment_var	*environment;
	int					has_env;

	environment = create_new_env_node("_?", "0");
	if (!environment)
		return (print_error_exec_message(ENVIRONMENT_INIT_FAILED, NULL),
			cleanup_all(environment, NULL, -1), NULL);
	environment->ctr_d_in_heredoc = 0;
	environment->last_exit_code = 0;
	if (!process_envp(environment, envp, &has_env))
		return (print_error_exec_message(ENVIRONMENT_INIT_FAILED, NULL),
			cleanup_all(environment, NULL, -1), NULL);
	if (!has_env && !add_minimal_env(environment))
		return (print_error_exec_message(ENVIRONMENT_INIT_FAILED, NULL),
			cleanup_all(environment, NULL, -1), NULL);
	return (environment);
}

t_environment_var	*initialize_shell(char **envp)
{
	t_environment_var	*environment;

	environment = init_environment(envp);
	if (!environment)
		return (NULL);
	return (environment);
}
