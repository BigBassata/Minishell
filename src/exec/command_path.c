/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 18:40:02 by licohen           #+#    #+#             */
/*   Updated: 2025/01/16 14:54:49 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

static char *build_command_path(const char *dir, const char *cmd)
{
    char *full_path;
    char *tmp;

    tmp = ft_strjoin(dir, DIR_SEPARATOR);
    if (!tmp)
        return (NULL);
    full_path = ft_strjoin(tmp, cmd);
    free(tmp);
    return (full_path);
}

static char **get_path_directories(t_environment_var *environment)
{
    char *path_value = get_env_value(environment, "PATH");
    if (!path_value)
        return (NULL);
    return ft_split(path_value, PATH_SEPARATOR);
}

static char *search_command_in_directories(const char *command, char **path_dirs)
{
    char *cmd_path;
    int i = 0;

    while (path_dirs[i])
    {
        cmd_path = build_command_path(path_dirs[i], command);
        if (!cmd_path || access(cmd_path, X_OK) == 0)
        {
            free_array(path_dirs);
            return (cmd_path);
        }
        free(cmd_path);
        i++;
    }
    return (NULL);
}

char *find_command_path(const char *command, t_environment_var *environment)
{
    char **path_dirs;
    char *cmd_path;

    if (check_command_type(command) == TRUE)
        return (ft_strdup(command));

    path_dirs = get_path_directories(environment);
    if (!path_dirs)
        return (NULL);

    cmd_path = search_command_in_directories(command, path_dirs);
    free_array(path_dirs);
    return (cmd_path);
}
