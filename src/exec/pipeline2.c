/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:08:50 by liamcohen         #+#    #+#             */
/*   Updated: 2025/03/05 17:44:51 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	init_next_pipe(int *next_pipe)
{
	next_pipe[0] = -1;
	next_pipe[1] = -1;
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

int	check_wait_error(pid_t wait_result)
{
	if (wait_result == -1)
	{
		if (errno != ECHILD)
		{
			print_error_exec_message(WAITPID_ERROR, NULL);
			return (ERROR);
		}
	}
	return (TRUE);
}
