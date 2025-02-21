/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liamcohen <liamcohen@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:32:13 by licohen           #+#    #+#             */
/*   Updated: 2025/02/11 12:12:15 by liamcohen        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <dirent.h>
# include <errno.h>
# include <limits.h>
# include <fcntl.h>
# include <string.h>
# include <sys/signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../Libft/include/libft.h"
# include "minishell_parsing.h"
# include "minishell_exec.h"
# include "minishell_struct.h"

extern int	g_signal;

// FUNCTIONS ////////////////////////////////////////////////////////////////
void		minishell_loop(t_environment_var *env);
t_command	*data_parsing(char *line, t_environment_var *env);

// ERROR
void		print_syntax_error(t_error error);
void		print_syntax_error_message(char *message);
char		*get_error_exec_message(t_error error);
void		print_error_exec_message(t_error error, char *word);
void		print_error_message(char *word);

char		*get_error_exec_message_2(t_error error);
char		*get_error_exec_message_3(t_error error);
void		handle_cd_errors(char *word);

// SIGNALS
void		interactive_mode_ctrl_c(int signal);
void		setup_signals_interactive_mode(t_environment_var *env);
void		exec_mode_ctrl_c(int signal);
void		exec_mode_sigquit(int signal);
void		setup_signals_exec_mode(void);
void		heredoc_mode_ctrl_c(int signal);
int			heredoc_sigint_event(void);
void		setup_signals_heredoc_mode(void);

// SIGNALS UTILS
void		handle_heredoc_ctr_d(char *delimiter, t_environment_var *env);

#endif