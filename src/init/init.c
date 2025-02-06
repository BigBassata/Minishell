/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:09:46 by licohen           #+#    #+#             */
/*   Updated: 2025/02/06 17:53:33 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_environment_var *create_environment_node(char *envp)
{
    t_environment_var   *new_node;
    char                *equals_sign;

    new_node = malloc(sizeof(t_environment_var));
    if (!new_node)
        return (NULL);
    equals_sign = ft_strchr(envp, '=');
    if (equals_sign)
    {
        new_node->key = ft_substr(envp, 0, equals_sign - envp);
        new_node->value = ft_strdup(equals_sign + 1);
    }
    else
    {
        new_node->key = ft_strdup(envp);
        new_node->value = ft_strdup("");
    }
    if (!new_node->key || !new_node->value)
    {
        cleanup_env_node(new_node);
        return (NULL);
    }
    new_node->next = NULL;
    return (new_node);
}

t_environment_var *init_environment(char **envp)
{
    t_environment_var   *environment;
    t_environment_var   *new_node;
    t_environment_var   *last_node;

    environment = NULL;
    last_node = NULL;
    while (*envp)
    {
        new_node = create_environment_node(*envp);
        if (!new_node)
        {
            print_error_exec_message(ENVIRONMENT_INIT_FAILED, NULL);
            cleanup_environment(environment);
            return (NULL);
        }
        if (!environment)
            environment = new_node;
        else
            last_node->next = new_node;
        last_node = new_node;
        envp++;
    }
    if (environment)
        environment->last_exit_code = 0;
    return (environment);
}

t_environment_var *initialize_shell(char **envp)
{
    return (init_environment(envp));
}