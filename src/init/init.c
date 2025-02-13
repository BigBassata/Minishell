/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:09:46 by licohen           #+#    #+#             */
/*   Updated: 2025/02/13 15:53:56 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// t_environment_var *create_environment_node(char *envp)
// {
//     t_environment_var   *new_node;
//     char                *equals_sign;

//     new_node = malloc(sizeof(t_environment_var));
//     if (!new_node)
//         return (NULL);
//     equals_sign = ft_strchr(envp, '=');
//     if (equals_sign)
//     {
//         new_node->key = ft_substr(envp, 0, equals_sign - envp);
//         new_node->value = ft_strdup(equals_sign + 1);
//     }
//     else
//     {
//         new_node->key = ft_strdup(envp);
//         new_node->value = ft_strdup("");
//     }
//     if (!new_node->key || !new_node->value)
//     {
//         cleanup_env_node(new_node);
//         return (NULL);
//     }
//     new_node->next = NULL;
//     return (new_node);
// }

// t_environment_var *init_environment(char **envp)
// {
//     t_environment_var   *environment;
//     t_environment_var   *new_node;
//     t_environment_var   *last_node;

//     environment = NULL;
//     last_node = NULL;
//     while (*envp)
//     {
//         new_node = create_environment_node(*envp);
//         if (!new_node)
//         {
//             print_error_exec_message(ENVIRONMENT_INIT_FAILED, NULL);
//             cleanup_environment(environment);
//             return (NULL);
//         }
//         if (!environment)
//             environment = new_node;
//         else
//             last_node->next = new_node;
//         last_node = new_node;
//         envp++;
//     }
//     if (environment)
//         environment->last_exit_code = 0;
//     return (environment);
// }

// t_environment_var *initialize_shell(char **envp)
// {
//     return (init_environment(envp));
// }

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
        cleanup_ptr(new_node->key);
        cleanup_ptr(new_node->value);
        cleanup_ptr(new_node);
        return (NULL);
    }
    return (new_node->next = NULL, new_node);
}

t_environment_var *init_environment(char **envp)
{
    t_environment_var   *environment;
    t_environment_var   *new_node;
    t_environment_var   *last_node;

    environment = create_new_env_node("_?", "");
    if (!environment)
        return (print_error_exec_message(ENVIRONMENT_INIT_FAILED, NULL),
            cleanup_all(environment, NULL, -1), NULL);
    environment->ctr_d_in_heredoc = 0;
    environment->last_exit_code = 0;
    last_node = environment;
    while (*envp)
    {
        new_node = create_environment_node(*envp);
        if (!new_node)
            return (print_error_exec_message(ENVIRONMENT_INIT_FAILED, NULL),
                cleanup_all(environment, NULL, -1), NULL);
        last_node->next = new_node;
        last_node = new_node;
        envp++;
    }
    return (environment);
}

t_environment_var *initialize_shell(char **envp)
{
    t_environment_var *environment;

    environment = init_environment(envp);
    if (!environment)
        return (NULL);
    return (environment);
}

void	del_environment(t_environment_var *envp)
{
	t_environment_var *temp;

	while (envp)
	{
		temp = envp;
		envp = envp->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}
