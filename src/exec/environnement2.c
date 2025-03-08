/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:37:12 by licohen           #+#    #+#             */
/*   Updated: 2025/02/18 15:33:14 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	is_valid_env_name(const char *name)
{
	int	i;

	if (!name || !*name || ft_isdigit(*name))
		return (FALSE);
	i = 0;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	is_readonly_var(const char *name)
{
	return (ft_strcmp(name, "_?") == 0);
}

t_environment_var	*create_new_env_node(const char *name, const char *value)
{
	t_environment_var	*new_node;

	new_node = malloc(sizeof(t_environment_var));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(name);
	if (!new_node->key)
		return (free(new_node), NULL);
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	if (value && !new_node->value)
		return (free(new_node->key), free(new_node), NULL);
	new_node->next = NULL;
	return (new_node);
}

static int	get_env_size_and_alloc(t_environment_var *environment,
	char ***env_array)
{
	t_environment_var	*current;
	int					env_size;

	env_size = 0;
	current = environment;
	while (current)
	{
		env_size++;
		current = current->next;
	}
	*env_array = malloc(sizeof(char *) * (env_size + 1));
	if (!*env_array)
		return (ERROR);
	(*env_array)[env_size] = NULL;
	return (env_size);
}

char	**convert_env_to_array(t_environment_var *environment)
{
	char				**env_array;
	t_environment_var	*current;
	char				*temp;
	int					i;

	i = get_env_size_and_alloc(environment, &env_array);
	if (i == ERROR)
		return (NULL);
	current = environment;
	i = 0;
	while (current)
	{
		temp = ft_strjoin(current->key, "=");
		if (!temp)
			return (free_array(env_array), NULL);
		env_array[i] = ft_strjoin(temp, current->value);
		free(temp);
		if (!env_array[i])
			return (free_array(env_array), NULL);
		current = current->next;
		i++;
	}
	return (env_array);
}
