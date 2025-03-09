/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 16:14:07 by licohen           #+#    #+#             */
/*   Updated: 2025/03/09 18:53:13 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static int	handle_exit_error(t_error error, char *word)
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

static int	is_within_long_limits(const char *str, int start_index)
{
	const char	*max_long;
	size_t		str_len;
	int			i;

	max_long = MAX_LONG;
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
			if (ft_strncmp(&str[start_index], MAX_LONG_PLUS_1, str_len) > 0)
				return (0);
		}
		else if (ft_strncmp(&str[start_index], max_long, str_len) > 0)
			return (0);
	}
	return (1);
}

static int	is_num(char *str)
{
	int	i;

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

static int check_exit_args(char **array, int len)
{
    char *endptr;
    long code;

    if (len == 1)
        return (0);
    if (!is_num(array[1]))
    {
        handle_exit_error(NUMERIC_ARGUMENT_REQUIRED, array[1]);
        return (2);
    }
    if (len > 2)
    {
        handle_exit_error(TOO_MANY_ARGUMENTS, "exit");
        return (-1);
    }
    code = ft_strtol_base_10(array[1], &endptr);
    if (*endptr != '\0')
    {
        handle_exit_error(NUMERIC_ARGUMENT_REQUIRED, array[1]);
        return (2);
    }
    return (code & 0xFF);
}

int ft_exit(t_command *cmd, int fd_out)
{
    int len;
    char **array;
    int exit_code;

    if (!cmd || !cmd->args)
        return (-1);
    array = cmd->args;
    len = nbr_of_args(array);
    if (fd_out < 0)
        fd_out = STDOUT_FILENO;
    exit_code = check_exit_args(array, len);
    if (exit_code >= 0)
        ft_putendl_fd("exit", fd_out);
    return (exit_code);
}
