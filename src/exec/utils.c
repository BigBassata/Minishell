/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:45:20 by licohen           #+#    #+#             */
/*   Updated: 2025/03/05 07:02:26 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

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
