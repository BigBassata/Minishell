/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 18:28:17 by licohen           #+#    #+#             */
/*   Updated: 2025/02/13 14:44:33 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"


int	is_builtin(char *cmd)
{
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

void	execute_builtin(t_command *cmd, t_environment_var **env)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (cmd->output_fd < 0)
        cmd->output_fd = STDOUT_FILENO;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		cmd->exit_code = ft_echo(cmd->args, cmd->output_fd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		cmd->exit_code = ft_cd(cmd->args, *env);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		cmd->exit_code = ft_pwd(cmd->output_fd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		cmd->exit_code = ft_export(cmd->args[1], *env, cmd->output_fd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		cmd->exit_code = ft_unset(env, cmd->args);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		cmd->exit_code = ft_env(*env, cmd->output_fd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		cmd->exit_code = ft_exit(cmd, cmd->output_fd);
	(*env)->last_exit_code = cmd->exit_code;
}

/* Le fonctionnement détaillé :

Vérification initiale :

Vérifie que cmd existe
Vérifie que cmd->args existe
Vérifie que cmd->args[0] existe (le nom de la commande)


Identification et exécution :

Compare cmd->args[0] avec chaque commande connue
Quand une correspondance est trouvée, exécute la fonction appropriée
Chaque fonction builtin reçoit les paramètres dont elle a besoin
Le résultat de l'exécution est stocké dans cmd->exit_status


Différences dans les appels :

echo, pwd : reçoivent juste les arguments
cd, export, unset, exit : reçoivent cmd, env, et args
env : reçoit juste l'environnement


Finalisation :

Le statut de sortie est copié dans l'environnement
Permet de le récupérer plus tard avec $? */
