/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:28:17 by licohen           #+#    #+#             */
/*   Updated: 2025/03/09 18:49:45 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (FALSE);
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

static void close_all_open_fds(void)
{
    int fd;

	fd = 3;
    while (fd < 256)
	{
		close(fd);
		fd++;
	}
}

int execute_builtin(t_command *cmd, t_environment_var **env)
{    
    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);
        
    if (cmd->output_fd < 0)
        cmd->output_fd = STDOUT_FILENO;
        
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        cmd->exit_code = ft_echo(cmd->args, cmd->output_fd);
    else if (ft_strcmp(cmd->args[0], "cd") == 0)
        cmd->exit_code = ft_cd(cmd->args, *env);
    else if (ft_strcmp(cmd->args[0], "pwd") == 0)
        cmd->exit_code = ft_pwd(cmd->output_fd);
    else if (ft_strcmp(cmd->args[0], "export") == 0)
        cmd->exit_code = ft_export_loop(cmd->args, *env, cmd->output_fd);
    else if (ft_strcmp(cmd->args[0], "unset") == 0)
        cmd->exit_code = ft_unset(env, cmd->args);
    else if (ft_strcmp(cmd->args[0], "env") == 0)
        cmd->exit_code = ft_env(cmd->args, *env, cmd->output_fd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		int exit_status = ft_exit(cmd, cmd->output_fd);
		if (exit_status >= 0) 
		{
			close_all_open_fds();
			cleanup_all(*env, cmd, exit_status) ;
			// exit(exit_status);
		}
		cmd->exit_code = exit_status;
	}
    (*env)->last_exit_code = cmd->exit_code;
    return (0);
}
