/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:02:22 by licohen           #+#    #+#             */
/*   Updated: 2025/03/09 22:15:25 by licohen          ###   ########.fr       */
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
	if (cmd_count == 0)
		info->process_ids = NULL;
	else
	{
		info->process_ids = malloc(sizeof(pid_t) * cmd_count);
		if (!info->process_ids)
			return (ERROR);
	}
	info->total_commands = cmd_count;
	info->current_index = 0;
	info->prev_pipe[0] = -1;
	info->prev_pipe[1] = -1;
	return (TRUE);
}

static	int	handle_builtin_pipeline(t_command *cmd, t_environment_var *env,
	t_pipeline_info *info)
{
	int	exit_status;

	exit_status = execute_builtin_parent(cmd, env);
	env->last_exit_code = exit_status;
	cleanup_pipeline(info);
	return (exit_status);
}

static	int	execute_commands_in_pipeline(t_command *cmd,
	t_environment_var *env, t_pipeline_info *info)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (execute_piped_command(current, env, info) == ERROR)
		{
			cleanup_pipeline(info);
			return (ERROR);
		}
		info->current_index++;
		current = current->next;
	}
	return (TRUE);
}

int	execute_pipeline(t_command *cmd, t_environment_var *environment)
{
	t_pipeline_info	info;
	int				exit_status;

	if (!cmd || !environment)
		return (ERROR);
	if (cmd && cmd->next == NULL && cmd->args && cmd->args[0]
		&& ft_strcmp(cmd->args[0], "exit") == 0)
	{
		info.process_ids = NULL;
		info.total_commands = 0;
		info.current_index = 0;
		info.prev_pipe[0] = -1;
		info.prev_pipe[1] = -1;
		return (handle_builtin_pipeline(cmd, environment, &info));
	}
	if (initialize_pipeline(&info, cmd) == ERROR)
		return (ERROR);
	if (cmd->next == NULL && check_builtin_execution(cmd))
		return (handle_builtin_pipeline(cmd, environment, &info));
	if (execute_commands_in_pipeline(cmd, environment, &info) == ERROR)
		return (ERROR);
	close_pipe_fds(info.prev_pipe);
	exit_status = wait_for_pipeline(&info);
	cleanup_pipeline(&info);
	return (exit_status);
}
