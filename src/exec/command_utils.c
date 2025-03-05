/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:48:52 by licohen           #+#    #+#             */
/*   Updated: 2025/03/05 01:21:44 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

enum e_state	check_command_type(const char *cmd)
{
	if (!cmd || !*cmd)
		return (ERROR);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (TRUE);
	return (FALSE);
}

t_cmd_type	get_command_type(t_command *cmd, t_environment_var *environment)
{
	char	*cmd_path;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (CMD_ERROR);
	if (is_builtin(cmd->args[0]))
		return (CMD_BUILTIN);
	cmd_path = find_command_path(cmd->args[0], environment);
	if (cmd_path)
	{
		free(cmd_path);
		return (CMD_EXTERNAL);
	}
	return (CMD_NOT_FOUND);
}

int	wait_for_child(pid_t pid)
{
	int	status;
	int	exit_status = 0;
	printf("wait_for_child: exit_status = %d\n", exit_status);
	if (waitpid(pid, &status, 0) == -1)
	{
		print_error_exec_message(WAITPID_ERROR, NULL);
		return (ERROR);
	}
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_status = 128 + WTERMSIG(status);
	else
		exit_status = ERROR;
	return (exit_status);
}
