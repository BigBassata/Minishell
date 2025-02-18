/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:42:02 by licohen           #+#    #+#             */
/*   Updated: 2025/02/18 15:53:19 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void    free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        array[i] = NULL;
        i++;
    }
    free(array);
}

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
