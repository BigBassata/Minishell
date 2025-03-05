/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:02:22 by licohen           #+#    #+#             */
/*   Updated: 2025/03/05 16:38:48 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	wait_for_pipeline(t_pipeline_info *info)
{
	int	last_status;
	int	i;

	if (!info || !info->process_ids || info->total_commands <= 0)
		return (0);
	last_status = 0;
	i = 0;
	while (i < info->total_commands)
	{
		if (info->process_ids[i] > 0)
		{
			if (handle_wait_status(info->process_ids[i],
					i == info->total_commands - 1,
					&last_status) == ERROR)
				return (ERROR);
		}
		i++;
	}
	return (last_status);
}

void	cleanup_pipeline(t_pipeline_info *info)
{
	if (info && info->process_ids)
	{
		free(info->process_ids);
		info->process_ids = NULL;
	}
}

void	close_pipe_fds(int *pipe_fds)
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

int	initialize_pipeline(t_pipeline_info *info, t_command *cmd)
{
	int			cmd_count;
	t_command	*current;

	cmd_count = 0;
	current = cmd;
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

int	execute_pipeline(t_command *cmd, t_environment_var *environment)
{
	t_pipeline_info	info;
	int				exit_status;
	t_command		*current;

	if (!cmd || !environment)
		return (ERROR);
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
	exit_status = wait_for_pipeline(&info);
	cleanup_pipeline(&info);
	return (exit_status);
}
