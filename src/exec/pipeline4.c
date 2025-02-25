/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:53:29 by liamcohen         #+#    #+#             */
/*   Updated: 2025/02/25 16:03:22 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	close_saved_fds(int stdin_fd, int stdout_fd)
{
	if (stdin_fd != -1)
		close(stdin_fd);
	if (stdout_fd != -1)
		close(stdout_fd);
}

int	restore_fds2(int stdin_fd, int stdout_fd)
{
	int	status;

	status = 0;
	if (dup2(stdin_fd, STDIN_FILENO) == -1)
		status = ERROR;
	if (dup2(stdout_fd, STDOUT_FILENO) == -1)
		status = ERROR;
	close_saved_fds(stdin_fd, stdout_fd);
	return (status);
}
