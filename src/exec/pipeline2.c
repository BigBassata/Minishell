/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:08:50 by liamcohen         #+#    #+#             */
/*   Updated: 2025/02/13 18:15:39 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int setup_child_process(t_command *cmd, t_pipeline_info *info, int next_pipe[2])
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
    return (setup_redirections(cmd));
}

static int prepare_command_execution(t_command *cmd, t_environment_var *env)
{
    char *cmd_path;
    char **env_array;

    if (is_builtin(cmd->args[0]))
    {
        execute_builtin(cmd, &env);
        exit(cmd->exit_code);
    }
    cmd_path = find_command_path(cmd->args[0], env);
    if (!cmd_path)
    {
        print_error_exec_message(COMMAND_NOT_FOUND, cmd->args[0]);
        exit(127);
    }
    env_array = convert_env_to_array(env);
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

int check_builtin_execution(t_command *cmd)
{
    return (cmd->next == NULL && is_builtin(cmd->args[0]) &&
            (ft_strcmp(cmd->args[0], "cd") == 0 || 
             ft_strcmp(cmd->args[0], "exit") == 0 ||
             ft_strcmp(cmd->args[0], "export") == 0 ||
             ft_strcmp(cmd->args[0], "unset") == 0));
}

static int handle_parent_process(t_pipeline_info *info, pid_t pid, int next_pipe[2])
{
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

int execute_piped_command(t_command *cmd, t_environment_var *env, 
                         t_pipeline_info *info)
{
    pid_t pid;
    int next_pipe[2] = {-1, -1};

    if (check_builtin_execution(cmd))
        return execute_builtin_parent(cmd, env);

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
    if (pid == 0)
    {
        if (setup_child_process(cmd, info, next_pipe) == ERROR)
            exit(ERROR);
        prepare_command_execution(cmd, env);
    }
    return (handle_parent_process(info, pid, next_pipe));
}
