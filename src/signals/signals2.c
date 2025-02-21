/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_mode_ctrl_c(int signal)
{
	(void)signal;
	g_signal = SIGINT;
	rl_done = 1;
	ft_printf("test\n");
}

int	heredoc_sigint_event(void)
{
	return (0);
}

void	setup_signals_heredoc_mode(void)
{
	rl_event_hook = heredoc_sigint_event;
	signal(SIGINT, heredoc_mode_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}
