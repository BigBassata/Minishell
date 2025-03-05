/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exec2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 16:48:52 by licohen           #+#    #+#             */
/*   Updated: 2025/03/05 16:38:33 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	wait_for_child(pid_t pid)
{
	int	status;
	int	exit_status;

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

int	handle_wait_status(pid_t pid, int is_last, int *last_status)
{
	int		status;
	pid_t	wait_result;

	wait_result = waitpid(pid, &status, 0);
	if (check_wait_error(wait_result) == ERROR)
		return (ERROR);
	if (is_last && wait_result != -1)
	{
		if (WIFEXITED(status))
			*last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*last_status = 128 + WTERMSIG(status);
		else
			*last_status = ERROR;
	}
	return (TRUE);
}

int	check_builtin_execution(t_command *cmd)
{
	return (cmd->next == NULL && is_builtin(cmd->args[0])
		&& (ft_strcmp(cmd->args[0], "cd") == 0
			|| ft_strcmp(cmd->args[0], "exit") == 0
			|| ft_strcmp(cmd->args[0], "export") == 0
			|| ft_strcmp(cmd->args[0], "unset") == 0));
}

int	execute_builtin_parent(t_command *cmd, t_environment_var *env)
{
	int	stdin_backup;
	int	stdout_backup;
	int	exit_code;

	stdin_backup = -1;
	stdout_backup = -1;
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		return (cleanup_fds(stdin_backup, stdout_backup), ERROR);
	if (setup_redirections(cmd) == ERROR)
		return (cleanup_fds(stdin_backup, stdout_backup), ERROR);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		cleanup_fds(stdin_backup, stdout_backup);
	execute_builtin(cmd, &env);
	exit_code = cmd->exit_code;
	if (ft_strcmp(cmd->args[0], "exit") != 0
		&& restore_fds2(stdin_backup, stdout_backup) == ERROR)
		exit_code = ERROR;
	return (exit_code);
}
