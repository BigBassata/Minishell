/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 15:09:46 by licohen           #+#    #+#             */
/*   Updated: 2025/01/28 20:30:03 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

t_command	*data_parsing(char *line, t_environment_var *env)
{
	t_token		*token_list;
	t_command	*cmd_list;

	token_list = tokenize_line(line);
	if (!token_list)
		return (NULL);
	setup_signals_heredoc_mode();
	cmd_list = parse_token_list(token_list, env);
	if (!cmd_list)
		return (NULL);
	cmd_list = expand_args(cmd_list, env);
	if (!cmd_list)
		return (NULL);
	return (cmd_list);
}

void	minishell_loop(t_environment_var *env)
{
	char		*line;
	t_command	*cmd_list;

	while (1)
	{
		setup_signals_interactive_mode(env);
		line = readline("minishell> ");
		env->last_exit_code = g_signal;
		env->ctr_d_in_heredoc = 0;
		if (is_null_line(line))
			break ;
		if (is_void_case(line))
			continue ;
		if (*line)
			add_history(line);
		cmd_list = data_parsing(line, env);
		if (!cmd_list)
			continue ;
		setup_signals_exec_mode();
		env->last_exit_code = execute_pipeline(cmd_list, env);
		free_cmd_list(cmd_list);
	}
	rl_clear_history();
	cleanup_all(env, NULL, 0);
}

int	main(int argc, char __attribute__((unused)) **argv, char **envp)
{
	t_environment_var	*env;

	if (!isatty(0) || !isatty(1))
		return (0);
	env = initialize_shell(envp);
	if (!env)
		return (1);
	if (argc == 1)
		minishell_loop(env);
	return (0);
}
