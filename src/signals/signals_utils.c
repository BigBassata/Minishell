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

int	ctrl_c_in_main_loop(char *line)
{
	if (g_signal == 1)
		return (free(line), TRUE);
	return (FALSE);
}

void	handle_heredoc_ctr_d(void)
{
	print_error_message("warning: here-document delimited "
		"by end-of-file (wanted delimiter)");
	setup_signals_interactive_mode();
}
