/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 18:35:57 by licohen           #+#    #+#             */
/*   Updated: 2025/02/13 17:38:10 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

char	*get_env_value(t_environment_var *environment, const char *key)
{
	if (!environment || !key)
		return (NULL);
	while (environment)
	{
		if (ft_strcmp(environment->key, key) == 0)
			return (environment->value);
		environment = environment->next;
	}
	return (NULL);
}

int	create_env_var(t_environment_var **environment, const char *name,
	const char *value)
{
	t_environment_var	*new_node;
	t_environment_var	*current;

	if (!is_valid_env_name(name) || is_readonly_var(name))
		return (ERROR);
	new_node = create_new_env_node(name, value);
	if (!new_node)
		return (ERROR);
	if (*environment == NULL)
	{
		*environment = new_node;
		return (TRUE);
	}
	current = *environment;
	while (current->next != NULL)
		current = current->next;
	current->next = new_node;
	return (TRUE);
}

int	custom_setenv(t_environment_var **environment, const char *name,
	const char *value)
{
	t_environment_var	*current;

	if (!environment || !name || !value)
		return (ERROR);
	if (!is_valid_env_name(name) || is_readonly_var(name))
		return (ERROR);
	current = *environment;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (ERROR);
			return (TRUE);
		}
		current = current->next;
	}
	return (create_env_var(environment, name, value));
}
