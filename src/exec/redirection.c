/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:02:02 by licohen           #+#    #+#             */
/*   Updated: 2025/02/13 15:52:16 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int handle_input_file(t_command *cmd)
{
    int input_fd;

    if (!cmd->input_path)
        return (TRUE);
    if (access(cmd->input_path, F_OK) == -1)
    {
        print_error_exec_message(FILE_NOT_FOUND, cmd->input_path);
        return (ERROR);
    }
    if (access(cmd->input_path, R_OK) == -1)
    {
        print_error_exec_message(PERMISSION_DENIED, cmd->input_path);
        return (ERROR);
    }
    input_fd = open(cmd->input_path, O_RDONLY);
    if (input_fd == -1)
    {
        print_error_exec_message(FILE_NOT_FOUND, cmd->input_path);
        return (ERROR);
    }
    if (dup2(input_fd, STDIN_FILENO) == -1)
    {
        close(input_fd);
        print_error_exec_message(DUP2_ERROR, cmd->input_path);
        return (ERROR);
    }
    close(input_fd);
    return (TRUE);
}

static int handle_output_file(t_command *cmd, int *prev_fds)
{
    int output_fd;
    int flags;

    if (!cmd->output_path)
        return (TRUE);
    if (cmd->is_append_mode)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    if (!check_output_permissions(cmd, prev_fds))
        return (ERROR);
    output_fd = open(cmd->output_path, flags, 0644);
    if (output_fd == -1)
    {
        print_error_exec_message(FILE_NOT_FOUND, cmd->output_path);
        return (ERROR);
    }
    if (dup2(output_fd, STDOUT_FILENO) == -1)
    {
        close(output_fd);
        print_error_exec_message(DUP2_ERROR, cmd->output_path);
        return (ERROR);
    }
    close(output_fd);
    return (TRUE);
}

int setup_redirections(t_command *cmd)
{
    int prev_fds[2];

    if (!cmd->input_path && !cmd->output_path)
        return (TRUE);
    if (setup_fd_backup(&prev_fds[0], &prev_fds[1]) == ERROR)
        return (ERROR);
    if (handle_input_file(cmd) == ERROR)
    {
        cleanup_fds(prev_fds[0], prev_fds[1]);
        return (ERROR);
    }
    if (handle_output_file(cmd, prev_fds) == ERROR)
    {
        cleanup_fds(prev_fds[0], prev_fds[1]);
        return (ERROR);
    }
    cleanup_fds(prev_fds[0], prev_fds[1]);
    return (TRUE);
}
