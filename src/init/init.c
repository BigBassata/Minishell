/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:09:46 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 20:48:34 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Crée un nouveau nœud d'environnement à partir d'une chaîne de format "KEY=VALUE"
** Cette fonction gère proprement la mémoire en cas d'échec
*/
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
        cleanup_env_node(new_node);  // Utilise notre nouvelle fonction spécialisée
        return (NULL);
    }
    new_node->next = NULL;
    return (new_node);
}

/*
** Initialise l'environnement complet à partir du tableau envp
** Gère la création de la liste chaînée et le nettoyage en cas d'erreur
*/
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
            cleanup_environment(environment);  // Utilise notre fonction spécialisée
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

/*
** Point d'entrée principal pour l'initialisation du shell
** Gère l'initialisation de l'environnement
*/
t_environment_var *initialize_shell(char **envp)
{
    return (init_environment(envp));  // Simplifié car une seule opération
}