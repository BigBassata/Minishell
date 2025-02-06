/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:37:12 by licohen           #+#    #+#             */
/*   Updated: 2025/02/06 17:52:25 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int is_valid_env_name(const char *name)
{
    int i;

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

int is_readonly_var(const char *name)
{
    return (ft_strcmp(name, "PWD") == 0 || ft_strcmp(name, "OLDPWD") == 0);
}

t_environment_var *create_new_env_node(const char *name, const char *value)
{
    t_environment_var *new_node;

    new_node = malloc(sizeof(t_environment_var));
    if (!new_node)
        return (NULL);
    new_node->key = ft_strdup(name);
    new_node->value = ft_strdup(value);
    new_node->next = NULL;
    if (!new_node->key || !new_node->value)
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return (NULL);
    }
    return (new_node);
}

char **convert_env_to_array(t_environment_var *environment)
{
    char **env_array;
    t_environment_var *current;
    int env_size;
    int i;
    char *temp;

    env_size = 0;
    current = environment;
    while (current)
    {
        env_size++;
        current = current->next;
    }
    env_array = malloc(sizeof(char *) * (env_size + 1));
    if (!env_array)
        return (NULL);
    i = 0;
    current = environment;
    while (current)
    {
        temp = ft_strjoin(current->key, "=");
        if (!temp)
        {
            free_array(env_array);
            return (NULL);
        }
        env_array[i] = ft_strjoin(temp, current->value);
        free(temp);
        if (!env_array[i])
        {
            free_array(env_array);
            return (NULL);
        }
        current = current->next;
        i++;
    }
    env_array[i] = NULL;
    return (env_array);
}
