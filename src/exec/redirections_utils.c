/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:28:10 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 19:47:15 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void cleanup_fds(int prev_in_fd, int prev_out_fd)
{
    if (prev_in_fd != -1)
        close(prev_in_fd);
    if (prev_out_fd != -1)
        close(prev_out_fd);
}

static void restore_fds(int *prev_fds)
{
    dup2(prev_fds[0], STDIN_FILENO);
    dup2(prev_fds[1], STDOUT_FILENO);
    cleanup_fds(prev_fds[0], prev_fds[1]);
}

int setup_fd_backup(int *prev_in_fd, int *prev_out_fd)
{
    *prev_in_fd = dup(STDIN_FILENO);
    *prev_out_fd = dup(STDOUT_FILENO);
    if (*prev_in_fd == -1 || *prev_out_fd == -1)
    {
        perror("minishell: dup");
        return (ERROR);
    }
    return (TRUE);
}

int check_input_permissions(t_command *cmd, int *prev_fds)
{
    if (access(cmd->input_path, F_OK) == -1)
    {
        print_error_exec_message(FILE_NOT_FOUND, cmd->input_path);
        cleanup_fds(prev_fds[0], prev_fds[1]);
        return (FALSE);
    }
    if (access(cmd->input_path, R_OK) == -1)
    {
        print_error_exec_message(PERMISSION_DENIED, cmd->input_path);
        cleanup_fds(prev_fds[0], prev_fds[1]);
        return (FALSE);
    }
    return (TRUE);
}

int check_output_permissions(t_command *cmd, int *prev_fds)
{
    char *dir_path;
    char *last_slash;

    dir_path = ft_strdup(cmd->output_path);
    if (!dir_path)
        return (ERROR);
    last_slash = ft_strrchr(dir_path, '/');
    if (last_slash)
    {
        *last_slash = '\0';
        if (access(dir_path, W_OK) == -1)
        {
            print_error_exec_message(PERMISSION_DENIED, cmd->output_path);
            cleanup_ptr(dir_path);
            restore_fds(prev_fds);
            return (FALSE);
        }
    }
    cleanup_ptr(dir_path);
    return (TRUE);
}

int setup_output_fd(t_command *cmd, int flags, int *prev_fds)
{
    cmd->output_fd = open(cmd->output_path, flags, 0644);
    if (cmd->output_fd == -1)
    {
        if (errno == EACCES)
            print_error_exec_message(PERMISSION_DENIED, cmd->output_path);
        else
            print_error_exec_message(FILE_NOT_FOUND, cmd->output_path);
        restore_fds(prev_fds);
        return (FALSE);
    }
    if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
    {
        print_error_exec_message(MEMORY_ALLOCATION_FAILED, NULL);
        close(cmd->output_fd);
        restore_fds(prev_fds);
        return (ERROR);
    }
    close(cmd->output_fd);
    return (TRUE);
}
