/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 18:14:27 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 18:44:52 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int handle_unset_error(t_error error, char *word)
{
    print_error_exec_message(error, word);
    return (1);
}

static void remove_env_var(t_environment_var **env, const char *name)
{
    t_environment_var   *current;
    t_environment_var   *prev;

    current = *env;
    prev = NULL;
    if (!name)
    return;
    while (current)
    {
        if (ft_strcmp(current->key, name) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return ;
        }
        prev = current;
        current = current->next;
    }
}
int ft_unset(t_environment_var **env, char **args)
{
    int i;
    int status;

    if (!env || !*env || !args)
        return (1);

    status = 0;
    i = 1;
    while (args[i])
    {
        if (!is_valid_env_name(args[i]))
            status = handle_unset_error(NOT_A_VALID_IDENTIFIER, args[i]);
        else if (is_readonly_var(args[i]))
            /* Vous pourriez ajouter READONLY_VARIABLE à votre enum */
            status = handle_unset_error(READONLY_VARIABLE, args[i]);
        else
            remove_env_var(env, args[i]);
        i++;
    }
    return (status);
}
