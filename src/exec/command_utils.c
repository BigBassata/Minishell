/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:48:52 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 19:43:01 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

enum e_state check_command_type(const char *cmd)
{
    if (!cmd || !*cmd)
        return (ERROR);
    if (cmd[0] == '/' || cmd[0] == '.')
        return (TRUE);
    return (FALSE);
}

// Détermine le type de commande (builtin, externe, ou erreur)
t_cmd_type get_command_type(t_command *cmd, t_environment_var *environment)
{
    char *cmd_path;

    if (!cmd || !cmd->args || !cmd->args[0])
        return (CMD_ERROR);
        
    // Vérifie si c'est une commande builtin
    if (is_builtin(cmd->args[0]))
        return (CMD_BUILTIN);
        
    // Vérifie si c'est une commande externe valide
    cmd_path = find_command_path(cmd->args[0], environment);
    if (cmd_path)
    {
        free(cmd_path);
        return (CMD_EXTERNAL);
    }
    
    return (CMD_NOT_FOUND);
}

// Attend la fin d'un processus enfant et retourne son statut de sortie
int wait_for_child(pid_t pid)
{
    int status;
    int exit_status;
    
    if (waitpid(pid, &status, 0) == -1)
    {
        print_error_exec_message(WAITPID_ERROR, NULL);
        return (ERROR);
    }
    
    // Si le processus s'est terminé normalement
    if (WIFEXITED(status))
        exit_status = WEXITSTATUS(status);
    // Si le processus a été terminé par un signal
    else if (WIFSIGNALED(status))
        exit_status = 128 + WTERMSIG(status);
    else
        exit_status = ERROR;
        
    return (exit_status);
}