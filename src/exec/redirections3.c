/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 14:28:10 by licohen           #+#    #+#             */
/*   Updated: 2025/03/05 07:02:10 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	build_heredoc_file_path(char *heredoc_file_path, char *input_path)
{
	int	path_len;
	int	file_len;

	ft_memset(heredoc_file_path, 0, 100);
	ft_strlcpy(heredoc_file_path, TMP_DIR, 100);
	ft_strlcat(heredoc_file_path, input_path, 100);
	path_len = ft_strlen(heredoc_file_path);
	file_len = ft_strlen(input_path);
	if (file_len > path_len
		|| ft_strncmp(&heredoc_file_path[path_len - file_len],
			input_path, file_len) != 0)
	{
		print_error_exec_message(HEREDOC_ERROR, input_path);
		return (ERROR);
	}
	return (TRUE);
}

int	handle_dup2_error(int fd, char *path)
{
	close(fd);
	print_error_exec_message(DUP2_ERROR, path);
	return (ERROR);
}