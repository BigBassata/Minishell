/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:14:07 by licohen           #+#    #+#             */
/*   Updated: 2025/02/20 14:47:50 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int handle_exit_error(t_error error, char *word)
{
    if (error == NUMERIC_ARGUMENT_REQUIRED)
    {
        print_error_exec_message(error, word);
        return (2);
    }
    else if (error == TOO_MANY_ARGUMENTS)
    {
        print_error_exec_message(error, word);
        return (1);
    }
    return (0);
}

static int is_within_long_limits(const char *str, int start_index)
{
    const char *max_long;
    size_t str_len;
    int i;

    max_long = "9223372036854775807";
    str_len = 0;
    i = start_index;
    while (str[i] && ft_isdigit(str[i]))
    {
        str_len++;
        i++;
    }
    if (str_len > ft_strlen(max_long))
        return (0);
    if (str_len == ft_strlen(max_long))
    {
        if (str[0] == '-')
        {
            if (ft_strncmp(&str[start_index], "9223372036854775808", str_len) > 0)
                return (0);
        }
        else if (ft_strncmp(&str[start_index], max_long, str_len) > 0)
            return (0);
    }
    return (1);
}

static int is_num(char *str)
{
    int i;

    if (!str)
        return (0);
    i = 0;
    while (ft_isspace(str[i]))
        i++;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (!str[i])
        return (0);
    if (!is_within_long_limits(str, i))
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int ft_exit(t_command *cmd, int fd_out)
{
    int len;
    long code;
    char *endptr;
    char **array;

    if (!cmd || !cmd->args)
        return (1);
    array = cmd->args;
    len = nbr_of_args(array);
    if (fd_out < 0)
        fd_out = STDOUT_FILENO;
    if (len == 1)
    {
        ft_putendl_fd("exit", fd_out);
        exit(0);
    }
    if (!is_num(array[1]))
        return (handle_exit_error(NUMERIC_ARGUMENT_REQUIRED, array[1]));
    if (len > 2)
        return (handle_exit_error(TOO_MANY_ARGUMENTS, "exit"));
    code = strtol(array[1], &endptr, 10);
    if (*endptr != '\0')
        return (handle_exit_error(NUMERIC_ARGUMENT_REQUIRED, array[1]));
    ft_putendl_fd("exit", fd_out);
    exit(code & 0xFF);
}
