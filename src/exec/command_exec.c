/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 18:40:26 by licohen           #+#    #+#             */
/*   Updated: 2025/02/17 20:01:02 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static void setup_child_signals(void)
{
    // Restaure le comportement par défaut des signaux pour les processus enfants
    signal(SIGINT, SIG_DFL);   // Ctrl+C termine le processus
    signal(SIGQUIT, SIG_DFL);  // Ctrl+\ génère un core dump
}

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

static int execute_child_process(char *cmd_path, t_command *cmd, char **env_array)
{
    if (cmd->output_fd != STDOUT_FILENO && cmd->output_fd > 0)
    {
        if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(ERROR);
        }
    }
    execve(cmd_path, cmd->args, env_array);
    perror("execve");
    exit(ERROR);
}

static int initialize_execution(t_command *cmd, t_environment_var *environment,
    char **cmd_path, char ***env_array)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return (ERROR);
    *cmd_path = find_command_path(cmd->args[0], environment);
    if (!*cmd_path)
        return (ERROR);
    *env_array = convert_env_to_array(environment);
    if (!*env_array)
    {
        free(*cmd_path);
        return (ERROR);
    }
    return (TRUE);
}

static int execute_external(t_command *cmd, t_environment_var *environment)
{
    char    *cmd_path;
    char    **env_array;
    pid_t   pid;
    int     status;

    if (initialize_execution(cmd, environment, &cmd_path, &env_array) == ERROR)
        return (ERROR);
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(cmd_path);
        free_array(env_array);
        return (ERROR);
    }
    if (pid == 0)
    {
        setup_child_signals();
        execute_child_process(cmd_path, cmd, env_array);
    }
    else 
    {
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);
    }
    free(cmd_path);
    free_array(env_array);
    status = wait_for_child(pid);
    setup_signals_interactive_mode(environment);
    return (status);
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
