/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:45:20 by licohen           #+#    #+#             */
/*   Updated: 2025/03/10 15:51:37 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	is_readonly_var(const char *name)
{
	return (ft_strcmp(name, "_?") == 0);
}

void	cleanup_fds(int prev_in_fd, int prev_out_fd)
{
	if (prev_in_fd != -1)
		close(prev_in_fd);
	if (prev_out_fd != -1)
		close(prev_out_fd);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

int	restore_fds2(int stdin_fd, int stdout_fd)
{
	int	status;

	status = 0;
	if (dup2(stdin_fd, STDIN_FILENO) == -1)
		status = ERROR;
	if (dup2(stdout_fd, STDOUT_FILENO) == -1)
		status = ERROR;
	if (stdin_fd != -1)
		close(stdin_fd);
	if (stdout_fd != -1)
		close(stdout_fd);
	return (status);
}

int	fork_process(pid_t *pid, int next_pipe[2])
{
	*pid = fork();
	if (*pid == -1)
	{
		print_error_exec_message(FORK_ERROR, NULL);
		close_pipe_fds(next_pipe);
		return (ERROR);
	}
	return (TRUE);
}
