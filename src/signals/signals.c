/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int g_signal = 0;

// void	interactive_mode_ctrl_c(int signal)
// {
// 	(void)signal;
// 	g_signal = 1;
// 	rl_replace_line("", 0);
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

// void	setup_signals_interactive_mode(void)
// {
// 	signal(SIGINT, interactive_mode_ctrl_c);
// 	signal(SIGQUIT, SIG_IGN);
// }

// void	heredoc_ctrl_c(int signal)
// {
// 	(void)signal;
// 	g_signal = 130;
// 	rl_replace_line("", 0);
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

// void	setup_signals_heredoc_mode(void)
// {
// 	signal(SIGINT, heredoc_ctrl_c);
// 	signal(SIGQUIT, SIG_IGN);
// }

// void	interactive_mode_ctrl_c(int signal)
// {
// 	if (g_signal != 130)
// 	{
// 		g_signal = signal;
// 		write(STDOUT_FILENO, "\n", 1);	
// 	}
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

void interactive_mode_ctrl_c(int signal)
{
    (void)signal;
    g_signal = 1;
    write(STDOUT_FILENO, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
}

void	setup_signals_interactive_mode(t_environment_var *env)
{
	if (env->ctr_d_in_heredoc)
		g_signal = 128 + SIGINT;
	else
		g_signal = 0;
	signal(SIGINT, interactive_mode_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}
