/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:02:22 by licohen           #+#    #+#             */
/*   Updated: 2025/02/25 00:20:39 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* pipeline_execution.c */

#include "minishell_exec.h"

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

int execute_builtin_parent(t_command *cmd, t_environment_var *env)
{
    int stdin_backup = -1;
    int stdout_backup = -1;
    int exit_code;
    
    stdin_backup = dup(STDIN_FILENO);
    stdout_backup = dup(STDOUT_FILENO);
    if (stdin_backup == -1 || stdout_backup == -1)
        return (close_saved_fds(stdin_backup, stdout_backup), ERROR);
    if (setup_redirections(cmd) == ERROR)
        return (close_saved_fds(stdin_backup, stdout_backup), ERROR);
    if (ft_strcmp(cmd->args[0], "exit") == 0)
        close_saved_fds(stdin_backup, stdout_backup);
    execute_builtin(cmd, &env);
    exit_code = cmd->exit_code;
    if (ft_strcmp(cmd->args[0], "exit") != 0 && 
        restore_fds2(stdin_backup, stdout_backup) == ERROR)
        exit_code = ERROR;
    return (exit_code);
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
	if (!cmd->next && check_builtin_execution(cmd))
		return (execute_builtin_parent(cmd, environment));
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
