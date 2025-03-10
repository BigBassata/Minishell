/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 18:40:26 by licohen           #+#    #+#             */
/*   Updated: 2025/03/10 16:10:13 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int	handle_pipeline_parent(t_pipeline_info *info, pid_t pid,
	int next_pipe[2])
{
	info->process_ids[info->current_index] = pid;
	if (info->prev_pipe[0] != -1)
	{
		close(info->prev_pipe[0]);
		close(info->prev_pipe[1]);
	}
	if (g_signal == 130)
	{
		close_pipe_fds(next_pipe);
		return (ERROR);
	}
	info->prev_pipe[0] = next_pipe[0];
	info->prev_pipe[1] = next_pipe[1];
	return (TRUE);
}

static int	setup_child_process(t_command *cmd, t_pipeline_info *info,
	int next_pipe[2])
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

static	int	prepare_command_execution(t_command *cmd, t_environment_var *env,
									t_pipeline_info *info)
{
	char	*cmd_path;
	char	**env_array;

	cmd_path = find_command_path(cmd->args[0], env);
	if (!cmd->args[0] || !cmd->args || !*cmd->args[0])
	{
		free(cmd_path);
		free(info->process_ids);
		cleanup_all(env, cmd, 0);
	}
	if (!cmd_path)
	{
		print_error_exec_message(COMMAND_NOT_FOUND, cmd->args[0]);
		free(info->process_ids);
		cleanup_all(env, cmd, 127);
	}
	env_array = convert_env_to_array(env);
	if (!env_array)
	{
		free(cmd_path);
		exit(ERROR);
	}
	execute_command(cmd_path, cmd->args, env_array, info);
	return (TRUE);
}

static	int	setup_and_execute_child(t_command *cmd, t_environment_var *env,
									t_pipeline_info *info, int next_pipe[2])
{
	if (setup_child_process(cmd, info, next_pipe) == ERROR)
	{
		free(info->process_ids);
		del_environment(env);
		cleanup_command(cmd);
		exit(1);
	}
	prepare_command_execution(cmd, env, info);
	return (TRUE);
}

int	execute_piped_command(t_command *cmd, t_environment_var *env,
						t_pipeline_info *info)
{
	pid_t	pid;
	int		next_pipe[2];

	if (cmd->is_heredoc && g_signal == 130)
		return (ERROR);
	init_next_pipe(next_pipe);
	if (check_builtin_execution(cmd))
		return (execute_builtin_parent(cmd, env));
	if (info->current_index < info->total_commands - 1)
	{
		if (pipe(next_pipe) == -1)
		{
			print_error_exec_message(PIPE_CREATION_ERROR, NULL);
			return (ERROR);
		}
	}
	if (fork_process(&pid, next_pipe) == ERROR)
		return (ERROR);
	if (pid == 0)
		setup_and_execute_child(cmd, env, info, next_pipe);
	return (handle_pipeline_parent(info, pid, next_pipe));
}
