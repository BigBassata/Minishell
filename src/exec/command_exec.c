/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 18:40:26 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 23:05:42 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int handle_command_error(t_command *cmd, char *command_path)
{
    if (!cmd->args[0] || !cmd->args[0][0])
    {
        print_error_exec_message(COMMAND_NOT_FOUND, "");
        return (ERROR);
    }

    if (!command_path)
    {
        print_error_exec_message(COMMAND_NOT_FOUND, cmd->args[0]);
        return (ERROR);
    }

    return (TRUE);
}

// static int execute_external(t_command *cmd, t_environment_var *environment)
// {
//     char *cmd_path;
//     char **env_array;
//     pid_t pid;

//     printf("\n=== Début de l'exécution externe ===\n");
//     printf("Commande à exécuter : %s\n", cmd->args[0]);
    
//     cmd_path = find_command_path(cmd->args[0], environment);
//     if (!cmd_path)
//     {
//         printf("Erreur : Impossible de trouver le chemin de la commande\n");
//         return (ERROR);
//     }
//     pid = fork();
//     if (pid == -1)
//     {
//         free(cmd_path);
//         return (ERROR);
//     }
//     if (pid == 0)
//     {
//         env_array = convert_env_to_array(environment);
//         execve(cmd_path, cmd->args, env_array);
//         free_array(env_array);
//         exit(ERROR);
//     }
//     free(cmd_path);
//     return (wait_for_child(pid));
// }
static int execute_external(t_command *cmd, t_environment_var *environment)
{
    char *cmd_path;
    char **env_array;
    pid_t pid;

    printf("\n=== DEBUG: Début de execute_external ===\n");
    printf("DEBUG: Vérifification des paramètres\n");
    if (!cmd || !cmd->args || !cmd->args[0])
    {
        printf("DEBUG: Paramètres invalides détectés\n");
        return (ERROR);
    }

    printf("DEBUG: Recherche du chemin de la commande '%s'\n", cmd->args[0]);
    cmd_path = find_command_path(cmd->args[0], environment);
    if (!cmd_path)
    {
        printf("DEBUG: Chemin non trouvé pour la commande\n");
        return (ERROR);
    }
    printf("DEBUG: Chemin trouvé: %s\n", cmd_path);

    printf("DEBUG: Création du tableau d'environnement\n");
    env_array = convert_env_to_array(environment);
    if (!env_array)
    {
        printf("DEBUG: Échec de la conversion de l'environnement\n");
        free(cmd_path);
        return (ERROR);
    }
    printf("DEBUG: Environnement converti avec succès\n");

    printf("DEBUG: Tentative de fork()\n");
    pid = fork();
    if (pid == -1)
    {
        printf("DEBUG: Fork a échoué\n");
        perror("fork");
        free(cmd_path);
        free_array(env_array);
        return (ERROR);
    }

    if (pid == 0)  // Processus enfant
    {
        printf("DEBUG: Dans le processus enfant\n");
        printf("DEBUG: Configuration des redirections\n");
        if (cmd->output_fd != STDOUT_FILENO)
        {
            printf("DEBUG: Redirection de la sortie vers fd %d\n", cmd->output_fd);
            if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                exit(ERROR);
            }
        }

        printf("DEBUG: Tentative d'exécution de '%s'\n", cmd_path);
        printf("DEBUG: Arguments:\n");
        for (int i = 0; cmd->args[i]; i++)
            printf("  args[%d] = '%s'\n", i, cmd->args[i]);

        execve(cmd_path, cmd->args, env_array);
        
        // Si on arrive ici, execve a échoué
        perror("execve");
        exit(ERROR);
    }

    // Processus parent
    printf("DEBUG: Dans le processus parent\n");
    printf("DEBUG: Nettoyage des ressources\n");
    free(cmd_path);
    free_array(env_array);
    
    printf("DEBUG: Attente du processus enfant\n");
    return (wait_for_child(pid));
}

int execute_command(t_command *cmd, t_environment_var *environment)
{
    t_cmd_type cmd_type;

    if (!cmd || !environment)
        return (ERROR);

    cmd_type = get_command_type(cmd, environment);
    if (cmd_type == CMD_ERROR)
        return (handle_command_error(cmd, NULL));
    if (cmd_type == CMD_NOT_FOUND)
        return (handle_command_error(cmd, cmd->args[0]));

    if (setup_redirections(cmd) == ERROR)
        return (ERROR);

    if (cmd_type == CMD_BUILTIN)
    {
        execute_builtin(cmd, &environment);
        return (cmd->exit_code);
    }
    else
        return (execute_external(cmd, environment));
}
