/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:45:20 by licohen           #+#    #+#             */
/*   Updated: 2025/02/20 14:58:42 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void cleanup_fds(int prev_in_fd, int prev_out_fd)
{
    if (prev_in_fd != -1)
        close(prev_in_fd);
    if (prev_out_fd != -1)
        close(prev_out_fd);
}

int handle_dup2_error(int fd, char *path)
{
    close(fd);
    print_error_exec_message(DUP2_ERROR, path);
    return (ERROR);
}

int handle_dup2_input_error(int fd, char *path)
{
    close(fd);
    print_error_exec_message(DUP2_ERROR, path);
    return (ERROR);
}

void setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}


int handle_external_parent(pid_t pid, t_environment_var *environment)
{
    int status;
    
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    status = wait_for_child(pid);
    setup_signals_interactive_mode(environment);
    return (status);
}