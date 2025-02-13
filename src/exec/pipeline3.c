/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:08:47 by liamcohen         #+#    #+#             */
/*   Updated: 2025/02/13 17:51:52 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int get_exit_status(int status)
{
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (ERROR);
}

static int check_wait_error(pid_t wait_result)
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

static int handle_wait_status(pid_t pid, int is_last, int *last_status)
{
    int status;
    pid_t wait_result;

    wait_result = waitpid(pid, &status, 0);
    if (check_wait_error(wait_result) == ERROR)
        return (ERROR);
    if (is_last && wait_result != -1)
        *last_status = get_exit_status(status);
    return (TRUE);
}

int wait_for_pipeline(t_pipeline_info *info)
{
    int last_status;
    int i;

    if (!info || !info->process_ids || info->total_commands <= 0)
        return (0);
    last_status = 0;
    for (i = 0; i < info->total_commands; i++)
    {
        if (info->process_ids[i] > 0)
        {
            if (handle_wait_status(info->process_ids[i], 
                i == info->total_commands - 1, 
                &last_status) == ERROR)
                return (ERROR);
        }
    }
    return (last_status);
}