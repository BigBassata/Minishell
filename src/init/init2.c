/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 00:05:37 by liamcohen         #+#    #+#             */
/*   Updated: 2025/02/25 16:10:25 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_default_env_var(t_environment_var **last_node, char *key, char *value)
{
	t_environment_var	*new_node;

	new_node = create_new_env_node(key, value);
	if (!new_node)
		return (FALSE);
	(*last_node)->next = new_node;
	*last_node = new_node;
	return (TRUE);
}

int	add_minimal_env(t_environment_var *environment)
{
	t_environment_var	*last_node;
	char				cwd[PATH_MAX];

	last_node = environment;
	while (last_node->next)
		last_node = last_node->next;
	if (!add_default_env_var(&last_node, "PATH",
			"/usr/local/bin:/usr/bin:/bin"))
		return (FALSE);
	if (!add_default_env_var(&last_node, "HOME", "/"))
		return (FALSE);
	if (getcwd(cwd, sizeof(cwd))
		&& !add_default_env_var(&last_node, "PWD", cwd))
		return (FALSE);
	return (TRUE);
}

int	process_envp(t_environment_var *env, char **envp, int *has_env)
{
	t_environment_var	*last_node;
	t_environment_var	*new_node;

	last_node = env;
	*has_env = (envp && *envp);
	while (envp && *envp)
	{
		new_node = create_environment_node(*envp);
		if (!new_node)
			return (FALSE);
		last_node->next = new_node;
		last_node = new_node;
		envp++;
	}
	return (TRUE);
}
