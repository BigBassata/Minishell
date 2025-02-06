/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:31:11 by licohen           #+#    #+#             */
/*   Updated: 2025/01/16 15:10:12 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(t_error error)
{
	if (error == SYNTAX_ERROR_REDIRECTION)
		ft_putendl_fd("minishell: syntax error near unexpected "
			"redirection token'", 2);
	else if (error == SYNTAX_ERROR_PIPE)
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
	else if (error == SYNTAX_ERROR_UNCLOSED_QUOTES)
		ft_putendl_fd("minishell: syntax error: unclosed quotes "
			"are not handled", 2);
}

void	print_syntax_error_message(char *message)
{
	ft_putstr_fd("minishell: syntax error: not handled token `", 2);
	if (message != NULL)
	{
		ft_putstr_fd(message, 2);
		ft_putendl_fd("'", 2);
	}
}

char	*get_error_exec_message(t_error error)
{
	if (error == COMMAND_NOT_FOUND)
		return ("command not found");
	else if (error == NOT_A_VALID_IDENTIFIER)
		return ("not a valid identifier");
	else if (error == NO_SUCH_FILE_OR_DIRECTORY)
		return ("No such file or directory");
	else if (error == NUMERIC_ARGUMENT_REQUIRED)
		return ("numeric argument required");
	else if (error == INVALID_OPTION)
		return ("invalid option");
	else if (error == TOO_MANY_ARGUMENTS)
		return ("too many arguments");
	else if (error == CD_NO_SUCH_FILE_OR_DIRECTORY)
		return ("No such file or directory");
	else if (error == CD_NOT_DIRECTORY)
		return ("Not a directory");
	else if (error == CD_NEED_RELATIVE_OR_ABSOLUTE_PATH)
		return ("Need relative or absolute path");
	else if (error == CD_ERR_RETRIEVING_CURR_DIRECTORY)
		return ("chdir: error retrieving current directory: getcwd: cannot "
			"access parent directories: No such file or directory");
	else if (error == CD_TOO_MANY_LVL_OF_SYMBOLIC_LINKS)
		return ("Too many levels of symbolic links.");
	return (get_error_exec_message_2(error));
}

void	print_error_exec_message(t_error error, char *word)
{
	ft_putstr_fd("minishell: ", 2);
	if (word != NULL)
		ft_putstr_fd(word, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(get_error_exec_message(error), 2);
}

void	print_error_message(char *word)
{
	ft_putstr_fd("minishell: ", 2);
	if (word != NULL)
		ft_putstr_fd(word, 2);
	ft_putendl_fd("", 2);
}
