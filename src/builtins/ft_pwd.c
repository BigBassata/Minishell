/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:12:09 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 18:44:48 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

/*
** ft_pwd : built-in pwd
** affiche le chemin du repertoire courant
*/

int ft_pwd(int fd_out)
{
    char dir[PATH_MAX];
    
    if (getcwd(dir, PATH_MAX))
    {
        ft_putendl_fd(dir, fd_out);
        return (0);
    }
    ft_putendl_fd("pwd: error retrieving current directory", 2);
    return (1);
}
