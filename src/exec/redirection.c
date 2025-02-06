/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:02:02 by licohen           #+#    #+#             */
/*   Updated: 2025/01/16 17:54:28 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int handle_heredoc_input(t_command *cmd, int *fd)
{
    *fd = open(cmd->input_path, O_RDONLY);
    if (*fd == -1)
    {
        print_error_exec_message(HEREDOC_ERROR, cmd->input_path);
        return (ERROR);
    }
    if (dup2(*fd, STDIN_FILENO) == -1)
    {
        print_error_exec_message(DUP2_ERROR, cmd->input_path);
        close(*fd);
        return (ERROR);
    }
    return (TRUE);
}

static int handle_regular_input(t_command *cmd, int *prev_fds)
{
    int fd;

    if (!check_input_permissions(cmd, prev_fds))
        return (FALSE);
    fd = open(cmd->input_path, O_RDONLY);
    if (fd == -1)
    {
        print_error_exec_message(FILE_NOT_FOUND, cmd->input_path);
        return (ERROR);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        print_error_exec_message(DUP2_ERROR, cmd->input_path);
        close(fd);
        return (ERROR);
    }
    close(fd);
    return (TRUE);
}

static int handle_input_redirection(t_command *cmd, int *prev_fds)
{
    int fd;

    if (!cmd->input_path)
        return (TRUE);
    if (ft_strncmp(cmd->input_path, ".heredoc_tmp", 11) == 0)
    {
        if (handle_heredoc_input(cmd, &fd) == ERROR)
            return (ERROR);
        if (fd != -1)
            close(fd);
        return (TRUE);
    }
    return (handle_regular_input(cmd, prev_fds));
}

static int handle_output_redirection(t_command *cmd, int *prev_fds)
{
    int flags;

    if (!cmd->output_path)
        return (TRUE);
    
    if (!check_output_permissions(cmd, prev_fds))
        return (FALSE);

    if (cmd->is_append_mode)
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else
        flags = O_WRONLY | O_CREAT | O_TRUNC;

    if (setup_output_fd(cmd, flags, prev_fds) == FALSE)
        return (ERROR);

    return (TRUE);
}

int setup_redirections(t_command *cmd)
{
    int prev_fds[2];
    int ret;

    if (setup_fd_backup(&prev_fds[0], &prev_fds[1]) == ERROR)
        return (ERROR);
    ret = handle_input_redirection(cmd, prev_fds);
    if (ret != TRUE)
    {
        cleanup_fds(prev_fds[0], prev_fds[1]);
        return (ret);
    }
    ret = handle_output_redirection(cmd, prev_fds);
    if (ret != TRUE)
    {
        cleanup_fds(prev_fds[0], prev_fds[1]);
        return (ret);
    }
    cleanup_fds(prev_fds[0], prev_fds[1]);
    return (TRUE);
}
