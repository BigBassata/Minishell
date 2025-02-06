/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*new_cmd(void)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)malloc(sizeof(t_command));
	if (new_cmd)
	{
		new_cmd->args = NULL;
		new_cmd->input_path = NULL;
		new_cmd->output_path = NULL;
		new_cmd->input_fd = -1;
		new_cmd->output_fd = -1;
		new_cmd->is_heredoc = 0;
		new_cmd->is_append_mode = 0;
		new_cmd->exit_code = -1;
		new_cmd->heredoc_delim = NULL;
		new_cmd->prev = NULL;
		new_cmd->next = NULL;
	}
	return (new_cmd);
}

void	add_last_cmd(t_command **first_cmd, t_command *new_cmd)
{
	t_command	*current;

	if (!*first_cmd)
		*first_cmd = new_cmd;
	else
	{
		current = *first_cmd;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
		new_cmd->prev = current;
	}
}

void	free_cmd_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
			free(args[i++]);
		free(args);
	}
}

void	free_cmd_list(t_command *first_cmd)
{
	t_command	*tmp;

	while (first_cmd)
	{
		tmp = first_cmd;
		first_cmd = first_cmd->next;
		if (first_cmd)
			first_cmd->prev = NULL;
		if (tmp->args)
			free_cmd_args(tmp->args);
		if (tmp->input_path && tmp->is_heredoc)
			unlink(tmp->input_path);
		if (tmp->input_path)
			free(tmp->input_path);
		if (tmp->output_path)
			free(tmp->output_path);
		if (tmp->heredoc_delim)
			free(tmp->heredoc_delim);
		tmp->next = NULL;
		free(tmp);
	}
}
