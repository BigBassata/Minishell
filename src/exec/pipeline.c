/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:02:22 by licohen           #+#    #+#             */
/*   Updated: 2025/02/13 16:30:59 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static void cleanup_pipeline(t_pipeline_info *info)
{
    if (info && info->process_ids)
    {
        free(info->process_ids);
        info->process_ids = NULL;
    }
}

static int initialize_pipeline(t_pipeline_info *info, t_command *cmd)
{
    int cmd_count = 0;
    t_command *current = cmd;

    while (current)
    {
        cmd_count++;
        current = current->next;
    }

    info->process_ids = malloc(sizeof(pid_t) * cmd_count);
    if (!info->process_ids)
        return (ERROR);

    info->total_commands = cmd_count;
    info->current_index = 0;
    info->prev_pipe[0] = -1;
    info->prev_pipe[1] = -1;
    return (TRUE);
}

static void close_pipe_fds(int *pipe_fds)
{
    if (pipe_fds[0] != -1)
    {
        close(pipe_fds[0]);
        pipe_fds[0] = -1;
    }
    if (pipe_fds[1] != -1)
    {
        close(pipe_fds[1]);
        pipe_fds[1] = -1;
    }
}

static int execute_builtin_parent(t_command *cmd, t_environment_var *env)
{
    int saved_stdin;
    int saved_stdout;
    int exit_code;

    exit_code = cmd->exit_code;
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdin == -1 || saved_stdout == -1)
        return (ERROR);
    if (setup_redirections(cmd) == ERROR)
    {
        close(saved_stdin);
        close(saved_stdout);
        return (ERROR);
    }
    execute_builtin(cmd, &env);
    dup2(saved_stdin, STDIN_FILENO);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdin);
    close(saved_stdout);
    return (exit_code);
}

static int execute_piped_command(t_command *cmd, t_environment_var *env, 
                               t_pipeline_info *info)
{
    if (info->total_commands == 1 && is_builtin(cmd->args[0]) &&
        (ft_strcmp(cmd->args[0], "cd") == 0 || 
         ft_strcmp(cmd->args[0], "exit") == 0 ||
         ft_strcmp(cmd->args[0], "export") == 0 ||
         ft_strcmp(cmd->args[0], "unset") == 0))
    {
        return execute_builtin_parent(cmd, env);
    }
    int next_pipe[2] = {-1, -1};
    pid_t pid;

    if (info->current_index < info->total_commands - 1)
    {
        if (pipe(next_pipe) == -1)
        {
            print_error_exec_message(PIPE_CREATION_ERROR, NULL);
            return (ERROR);
        }
    }

    pid = fork();
    if (pid == -1)
    {
        print_error_exec_message(FORK_ERROR, NULL);
        close_pipe_fds(next_pipe);
        return (ERROR);
    }

    if (pid == 0)  // Processus enfant
    {
        if (info->prev_pipe[0] != -1)
        {
            dup2(info->prev_pipe[0], STDIN_FILENO);
            close(info->prev_pipe[0]);
            close(info->prev_pipe[1]);
        }

        if (next_pipe[1] != -1)
        {
            dup2(next_pipe[1], STDOUT_FILENO);
            close(next_pipe[0]);
            close(next_pipe[1]);
        }

        if (setup_redirections(cmd) == ERROR)
            exit(ERROR);

        if (is_builtin(cmd->args[0]))
        {
            execute_builtin(cmd, &env);
            exit(cmd->exit_code);
        }
        else
        {
            char *cmd_path = find_command_path(cmd->args[0], env);
            if (!cmd_path)
            {
                print_error_exec_message(COMMAND_NOT_FOUND, cmd->args[0]);
                exit(ERROR);
            }

            char **env_array = convert_env_to_array(env);
            if (!env_array)
            {
                free(cmd_path);
                exit(ERROR);
            }

            execve(cmd_path, cmd->args, env_array);
            perror("execve");
            free(cmd_path);
            free_array(env_array);
            exit(ERROR);
        }
    }

    info->process_ids[info->current_index] = pid;
    
    if (info->prev_pipe[0] != -1)
    {
        close(info->prev_pipe[0]);
        close(info->prev_pipe[1]);
    }
    
    info->prev_pipe[0] = next_pipe[0];
    info->prev_pipe[1] = next_pipe[1];

    return (TRUE);
}

static int wait_for_pipeline(t_pipeline_info *info)
{
    int last_status = 0;
    int i;

    if (!info || !info->process_ids || info->total_commands <= 0)
        return (0);

    for (i = 0; i < info->total_commands; i++)
    {
        if (info->process_ids[i] > 0)  // Ne faire waitpid que pour les PID valides
        {
            int status;
            pid_t wait_result = waitpid(info->process_ids[i], &status, 0);
            
            if (wait_result == -1)
            {
                if (errno != ECHILD)  // Ignorer l'erreur si le processus n'existe pas
                {
                    print_error_exec_message(WAITPID_ERROR, NULL);
                    return (ERROR);
                }
            }
            else if (i == info->total_commands - 1)  // Statut de la dernière commande
            {
                if (WIFEXITED(status))
                    last_status = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                    last_status = 128 + WTERMSIG(status);
            }
        }
    }

    return (last_status);
}

int execute_pipeline(t_command *cmd, t_environment_var *environment)
{
    t_pipeline_info info;
    int exit_status;
    t_command *current;

    if (!cmd || !environment)
        return (ERROR);
    if (!cmd->next && is_builtin(cmd->args[0]) &&
        (ft_strcmp(cmd->args[0], "cd") == 0 || 
         ft_strcmp(cmd->args[0], "exit") == 0 ||
         ft_strcmp(cmd->args[0], "export") == 0 ||
         ft_strcmp(cmd->args[0], "unset") == 0))
    {
        return execute_builtin_parent(cmd, environment);
    }
    if (initialize_pipeline(&info, cmd) == ERROR)
        return (ERROR);
    current = cmd;
    while (current)
    {
        if (execute_piped_command(current, environment, &info) == ERROR)
        {
            cleanup_pipeline(&info);
            return (ERROR);
        }
        info.current_index++;
        current = current->next;
    }
    close_pipe_fds(info.prev_pipe);
        if (info.process_ids && info.current_index > 0)
    {
        exit_status = wait_for_pipeline(&info);
        cleanup_pipeline(&info);
        return (exit_status);
    }
    cleanup_pipeline(&info);
    return (0);
}
