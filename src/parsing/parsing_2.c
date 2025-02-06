/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_heredoc(t_command *curr_cmd, t_token *token, int *file_nb,
	t_environment_var *env)
{
	if (!curr_cmd || !token)
		return (ERROR);
	curr_cmd->is_heredoc = 1;
	if (curr_cmd->input_path)
		free(curr_cmd->input_path);
	curr_cmd->input_path = NULL;
	if (curr_cmd->heredoc_delim)
		free(curr_cmd->heredoc_delim);
	curr_cmd->heredoc_delim = NULL;
	if (token->next && token->next->type != TYPE_PIPE && token->next->value)
	{
		curr_cmd->heredoc_delim = ft_strdup(token->next->value);
		if (!curr_cmd->heredoc_delim)
			return (ERROR);
		if (refine_delimiter(&curr_cmd->heredoc_delim) == ERROR)
			return (ERROR);
		curr_cmd->input_path = handle_here_doc(file_nb,
				curr_cmd->heredoc_delim, env);
		free(token->next->value);
		token->next->value = NULL;
	}
	if (!curr_cmd->input_path)
		return (ERROR);
	return (TRUE);
}

int	parse_outfile(t_command *curr_cmd, t_token *tok)
{
	if (!curr_cmd || !tok)
		return (ERROR);
	while (tok && tok->type != TYPE_PIPE)
	{
		if (tok->type == TYPE_REDIR_OUT || tok->type == TYPE_REDIR_APPEND)
		{
			if (tok->type == TYPE_REDIR_OUT)
				curr_cmd->is_append_mode = 0;
			else if (tok->type == TYPE_REDIR_APPEND)
				curr_cmd->is_append_mode = 1;
			if (curr_cmd->output_path)
				free(curr_cmd->output_path);
			curr_cmd->output_path = NULL;
			if (tok->next && tok->next->type != TYPE_PIPE && tok->next->value)
			{
				curr_cmd->output_path = ft_strdup(tok->next->value);
				free(tok->next->value);
				tok->next->value = NULL;
			}
			if (!curr_cmd->output_path)
				return (print_error_message("parse outfile error"), ERROR);
		}
		tok = tok->next;
	}
	return (TRUE);
}

int	args_arr_counter(t_token *token)
{
	int		args_nb;

	args_nb = 0;
	while ((token && token->type != TYPE_PIPE))
	{
		if (token->type == TYPE_WORD && token->value)
			args_nb++;
		token = token->next;
	}
	return (args_nb);
}

char	**init_args_arr(t_token *token)
{
	char	**args_arr;
	int		args_nb;

	if (!token)
		return (NULL);
	args_nb = args_arr_counter(token);
	args_arr = (char **)malloc((args_nb + 1) * sizeof(char *));
	if (!args_arr)
		return (NULL);
	args_arr[args_nb] = NULL;
	return (args_arr);
}

int	parse_cmd_args(t_command *curr_cmd, t_token *token)
{
	int		args_nb;
	int		i;

	if (!curr_cmd || !token)
		return (ERROR);
	args_nb = args_arr_counter(token);
	curr_cmd->args = init_args_arr(token);
	if (!curr_cmd->args)
		return (print_error_message("parse cmd args error"), ERROR);
	i = 0;
	while (token && token->type != TYPE_PIPE && i < args_nb)
	{
		if (token->type == TYPE_WORD && token->value)
		{
			curr_cmd->args[i] = ft_strdup(token->value);
			if (!curr_cmd->args[i])
				return (print_error_message("parse cmd args error"), ERROR);
			i++;
		}
		token = token->next;
	}
	return (TRUE);
}
