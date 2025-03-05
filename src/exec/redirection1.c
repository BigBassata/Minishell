/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:02:02 by licohen           #+#    #+#             */
/*   Updated: 2025/03/05 00:15:57 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int	handle_heredoc_input(t_command *cmd)
{
	int		input_fd;
	char	heredoc_file_path[100];

	if (!cmd->heredoc_delim || !cmd->input_path)
		return (TRUE);
	if (build_heredoc_file_path(heredoc_file_path, cmd->input_path) == ERROR)
		return (ERROR);
	input_fd = open(heredoc_file_path, O_RDONLY);
	if (input_fd == -1)
	{
		print_error_exec_message(HEREDOC_ERROR, cmd->heredoc_delim);
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

static int	handle_regular_input(t_command *cmd)
{
	int	input_fd;

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
		return (handle_dup2_error(input_fd, cmd->input_path));
	close(input_fd);
	return (TRUE);
}

static int	handle_input_file(t_command *cmd)
{
	if (!cmd->input_path)
		return (TRUE);
	if (cmd->is_heredoc)
		return (handle_heredoc_input(cmd));
	else
		return (handle_regular_input(cmd));
}

static int	handle_output_file(t_command *cmd, int *prev_fds)
{
	int	output_fd;
	int	flags;

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
		return (handle_dup2_error(output_fd, cmd->output_path));
	close(output_fd);
	return (TRUE);
}

int	setup_redirections(t_command *cmd)
{
	int	prev_fds[2];
	int	ret;

	if (!cmd->input_path && !cmd->output_path)
		return (TRUE);
	if (setup_fd_backup(&prev_fds[0], &prev_fds[1]) == ERROR)
		return (ERROR);
	ret = handle_input_file(cmd);
	if (ret != TRUE)
	{
		cleanup_fds(prev_fds[0], prev_fds[1]);
		return (ret);
	}
	ret = handle_output_file(cmd, prev_fds);
	if (ret != TRUE)
	{
		cleanup_fds(prev_fds[0], prev_fds[1]);
		return (ret);
	}
	cleanup_fds(prev_fds[0], prev_fds[1]);
	return (TRUE);
}
