/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 13:43:45 by licohen           #+#    #+#             */
/*   Updated: 2025/02/18 15:55:26 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_exec.h"

/*
** ft_echo : built-in echo
** Cette fonction vérifie si une option -n est valide
** Une option -n valide commence par '-' suivi d'un ou plusieurs 'n'
** Retourne 1 si l'option est valide, 0 sinon
*/
static int is_valid_n_option(const char *str)
{
    size_t i;

    if (!str)
        return (0);
    if (str[0] != '-' || str[1] != 'n')
        return (0);
    i = 2;
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

/*
** Cette fonction gère le cas où echo est appelé sans arguments
** Elle imprime simplement un retour à la ligne
** Retourne toujours 0 comme spécifié par la norme POSIX
*/
static int handle_no_args(int fd_out)
{
    ft_putchar_fd('\n', fd_out);
    return (0);
}

/*
** Cette fonction gère l'impression des arguments
** Elle imprime chaque argument suivi d'un espace si ce n'est pas le dernier
** Retourne le nombre d'arguments imprimés
*/

static void print_args(char **array, int start_index, int fd_out)
{
    int i;
    char *arg;
    
    i = start_index;
    while (array[i])
    {
        arg = array[i];
        if (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"')
        {
            arg++;
            arg[ft_strlen(arg) - 1] = '\0';
        }
        
        ft_putstr_fd(arg, fd_out);
        if (array[i + 1])
            ft_putchar_fd(' ', fd_out);
        i++;
    }
}

int ft_echo(char **array, int fd_out)
{
    int i;
    int has_n_option;

    if (!array)
        return (1);
    if (nbr_of_args(array) < 2)
        return (handle_no_args(fd_out));
    if (fd_out < 0)
        fd_out = STDOUT_FILENO;

    has_n_option = 0;
    i = 1;
    while (array[i] && is_valid_n_option(array[i]))
    {
        has_n_option = 1;
        i++;
    }
    print_args(array, i, fd_out);
    if (!has_n_option)
        ft_putchar_fd('\n', fd_out);

    return (0);
}