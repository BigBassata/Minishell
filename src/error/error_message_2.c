/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:31:11 by licohen           #+#    #+#             */
/*   Updated: 2025/01/16 15:10:12 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_error_exec_message_2(t_error error)
{
	if (error == OLDPWD_NOT_SET)
		return ("OLDPWD is not set");
	if (error == HOME_NOT_SET)
		return ("HOME is not set");
	else if (error == ENVIRONMENT_INIT_FAILED)
		return ("environment initialization failed");
	else if (error == MEMORY_ALLOCATION_FAILED)
		return ("memory allocation failed");
	else if (error == FILE_NOT_FOUND)
		return ("No such file or directory");
	else if (error == PERMISSION_DENIED)
		return ("Permission denied");
	else if (error == DUP2_ERROR)
		return ("dup2 failed");
	else if (error == PIPE_ERROR)
		return ("pipe error");
	else if (error == FORK_ERROR)
		return ("fork failed");
	else if (error == PIPE_CREATION_ERROR)
		return ("pipe creation failed");
	else if (error == WAITPID_ERROR)
		return ("waitpid failed");
	else if (error == HEREDOC_ERROR)
		return ("heredoc failed");
	return (get_error_exec_message_3(error));
}

char	*get_error_exec_message_3(t_error error)
{
	if (error == READONLY_VARIABLE)
		return ("cannot modify a read-only variable");
	return ("");
}

void	handle_cd_errors(char *word)
{
	char	message[PATH_MAX];

	ft_strlcpy(message, "cd: ", PATH_MAX);
	ft_strlcat(message, word, PATH_MAX);
	if (errno == ENOENT)
		print_error_exec_message(CD_NO_SUCH_FILE_OR_DIRECTORY, message);
	else if (errno == ENOTDIR)
		print_error_exec_message(CD_NOT_DIRECTORY, message);
	else if (errno == EACCES)
		print_error_exec_message(PERMISSION_DENIED, message);
	else if (errno == ELOOP)
		print_error_exec_message(CD_TOO_MANY_LVL_OF_SYMBOLIC_LINKS, message);
	else
		print_error_message("cd: error chdir");
}
