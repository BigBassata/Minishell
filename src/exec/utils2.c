/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:45:20 by licohen           #+#    #+#             */
/*   Updated: 2025/02/20 14:58:42 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

void	init_next_pipe(int *next_pipe)
{
	next_pipe[0] = -1;
	next_pipe[1] = -1;
}

int	is_running_programm(char *cmd, char *prog_name)
{
	size_t	cmd_len;
	size_t	prog_len;
	char	*prog_found;

	if (!cmd || !prog_name)
		return (FALSE);
	cmd_len = ft_strlen(cmd);
	prog_len = ft_strlen(prog_name);
	if ((ft_strncmp(cmd, "./", 2) != 0 && ft_strncmp(cmd, "/", 1) != 0
		&& ft_strncmp(cmd, "../", 3) != 0))
		return (FALSE);
	prog_found = ft_strnstr(cmd, prog_name, cmd_len);
	if (!prog_found)
		return (FALSE);
	if (prog_found[prog_len] != '\0' && prog_found[prog_len] != ' ')
		return (FALSE);
	return (TRUE);
}
