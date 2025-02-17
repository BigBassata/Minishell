#include "minishell.h"

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
	cmd_list = parse_token_list(token_list, env);
	if (!cmd_list)
		return (NULL);
	cmd_list = expand_args(cmd_list, env);
	if (!cmd_list)
		return (NULL);
	return (cmd_list);
}
    
int main(int __attribute__((unused)) argc, 
		char __attribute__((unused)) **argv, 
		char **envp)
{
	char *line;
	t_command *cmd_list;
	t_environment_var *env;
	env = initialize_shell(envp);
	if (!env)
		return (1);
		
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
		env->last_exit_code = execute_pipeline(cmd_list, env);
		free_cmd_list(cmd_list);
	}
	rl_clear_history();
	cleanup_all(env, NULL, 0);
	return (0);
}
