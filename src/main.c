#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:09:46 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 20:30:03 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Cette fonction gère l'exécution d'un pipeline de commandes
** Elle sera développée plus tard pour gérer les pipes correctement
*/
static int execute_pipeline(t_command *cmd_list, t_environment_var *env)
{
    int exit_code;
    t_command *current;

    current = cmd_list;
    exit_code = 0;
    
    // Pour l'instant, exécute la première commande
    // Cette partie sera étendue pour gérer les pipes
    if (current)
        exit_code = execute_command(current, env);

    return (exit_code);
}

/*
** Fonction principale qui gère la boucle du shell
** Elle intègre le parsing de votre binôme avec votre partie exécution
*/
int main(int __attribute__((unused)) argc, 
         char __attribute__((unused)) **argv, 
         char **envp)
{
    char *line;
    t_token *token_list;
    t_command *cmd_list;
    t_environment_var *env;

    // Initialisation de l'environnement
    env = initialize_shell(envp);
    if (!env)
        return (1);

    while (1)
    {
        // Lecture de l'entrée avec gestion du Ctrl+D
        line = readline("minishell> ");
        if (!line)
        {
            ft_printf("exit\n");
            break;
        }

        // Gestion des lignes vides et ajout à l'historique
        if (is_null_line(line) || is_void_case(line))
            continue;
        add_history(line);

        // Validation et filtrage de la ligne
        line = filtered_line(line);
        if (!line)
            continue;

        // Tokenisation de la ligne
        token_list = tokenize_line(line);
        if (!token_list)
            continue;

        // Parsing des tokens en commandes
        cmd_list = parse_token_list(token_list, env);
        if (!cmd_list)
            continue;
        printf("Command structure:\n");
        printf("Command: %s\n", cmd_list->args[0]);  // Devrait être "echo"
        printf("First arg: %s\n", cmd_list->args[1]); // Devrait être votre texte
        printf("Output FD: %d\n", cmd_list->output_fd); // Important pour la sortie!
        // Expansion des variables et suppression des quotes
        if (!expand_args(cmd_list, env))
        {
            free_cmd_list(cmd_list);
            continue;
        }

        // Exécution du pipeline de commandes
        env->last_exit_code = execute_pipeline(cmd_list, env);

        // Nettoyage après l'exécution
        free_cmd_list(cmd_list);
    }

    // Nettoyage final
    cleanup_all(env, NULL, 0);
    clear_history();
    return (0);
}