/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_cmd_list(t_token *token_list)
{
	t_command	*first_cmd;
	t_command	*last_cmd;
	int			pipes_nb;

	pipes_nb = 0;
	first_cmd = new_cmd();
	if (!first_cmd)
		return (NULL);
	while (token_list)
	{
		if (token_list->type == TYPE_PIPE)
			pipes_nb++;
		token_list = token_list->next;
	}
	while (pipes_nb-- > 0)
	{
		last_cmd = new_cmd();
		if (!last_cmd)
			return (free_cmd_list(first_cmd), NULL);
		add_last_cmd(&first_cmd, last_cmd);
	}
	return (first_cmd);
}

int	parse_infile(t_command *curr_cmd, t_token *token)
{
	if (!curr_cmd || !token)
		return (ERROR);
	curr_cmd->is_heredoc = 0;
	if (curr_cmd->input_path)
		free(curr_cmd->input_path);
	curr_cmd->input_path = NULL;
	if (curr_cmd->heredoc_delim)
		free(curr_cmd->heredoc_delim);
	curr_cmd->heredoc_delim = NULL;
	if (token->next && token->next->type != TYPE_PIPE && token->next->value)
	{
		curr_cmd->input_path = ft_strdup(token->next->value);
		free(token->next->value);
		token->next->value = NULL;
	}
	if (!curr_cmd->input_path)
		return (ERROR);
	return (TRUE);
}

int	parse_infile_and_heredoc(t_command *curr_cmd, t_token *token_list,
	int *file_nb, t_environment_var *env)
{
	if (!curr_cmd || !token_list)
		return (ERROR);
	while (token_list && token_list->type != TYPE_PIPE)
	{
		if (token_list->type == TYPE_REDIR_IN)
		{
			if (parse_infile(curr_cmd, token_list) == ERROR)
				return (print_error_message("parse infile error"), ERROR);
		}
		else if (token_list->type == TYPE_HEREDOC)
		{
			if (parse_heredoc(curr_cmd, token_list, file_nb, env) == ERROR)
				return (print_error_message("parse heredoc error"), ERROR);
		}
		token_list = token_list->next;
	}
	return (TRUE);
}

t_command	*handle_parsing(t_token *token_list, t_environment_var *env)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	int			file_nb;

	cmd_list = init_cmd_list(token_list);
	if (!cmd_list)
		return (NULL);
	current_cmd = cmd_list;
	file_nb = 1;
	while (current_cmd)
	{
		if (parse_infile_and_heredoc(current_cmd, token_list,
				&file_nb, env) == ERROR)
			return (free_cmd_list(cmd_list), NULL);
		if (parse_outfile(current_cmd, token_list) == ERROR)
			return (free_cmd_list(cmd_list), NULL);
		if (parse_cmd_args(current_cmd, token_list) == ERROR)
			return (free_cmd_list(cmd_list), NULL);
		current_cmd = current_cmd->next;
		if (current_cmd)
			moove_to_next_pipe(&token_list);
	}
	return (cmd_list);
}

t_command	*parse_token_list(t_token *token_list, t_environment_var *env)
{
	t_command	*cmd_list;

	if (!token_list)
		return (free_token_list(token_list), NULL);
	cmd_list = handle_parsing(token_list, env);
	free_token_list(token_list);
	return (cmd_list);
}
