/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc_ctr_d(char *delimiter, t_environment_var *env)
{
	g_signal = 128 + SIGINT;
	env->ctr_d_in_heredoc = 1;
	ft_putstr_fd("minishell: warning: here-document delimited "
		"by end-of-file (wanted `", 2);
	if (delimiter != NULL)
		ft_putstr_fd(delimiter, 2);
	else
		ft_putstr_fd("delimiter", 2);
	ft_putendl_fd("')", 2);
}
