/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	last_status_len(int exit_code)
{
	int	len;

	len = 0;
	while (exit_code > 9)
	{
		exit_code /= 10;
		len++;
	}
	if (exit_code >= 0 && exit_code <= 9)
		len++;
	return (len);
}

void	expand_last_status(char *dest, int *index, int exit_code,
	char *base_10)
{
	printf("DEBUG: expand_last_status called with exit_code = %d\n", exit_code);
	if (exit_code >= 0)
	{
		if (exit_code > 9)
			expand_last_status(dest, index, exit_code / 10, base_10);
		dest[(*index)++] = base_10[exit_code % 10];
	}
}

void	cpy_expanded_var(char *dest, int *index, char *env_var,
	t_environment_var *env)
{
	t_environment_var	*curr_env;
	char				*env_var_name;
	int					k;
	int					env_var_len;
    printf("DEBUG: cpy_expanded_var with env->last_exit_code = %d\n", env->last_exit_code);
	curr_env = env;
	env_var_name = &env_var[1];
	env_var_len = ft_strlen(env_var_name);
	if (env_var_name[0] == '?' && env_var_name[1] == '\0' && env)
		expand_last_status(dest, index, env->last_exit_code, "0123456789");
	while (curr_env)
	{
		if (ft_strncmp(curr_env->key, env_var_name, env_var_len) == 0
			&& curr_env->key[env_var_len] == '\0' && curr_env->value)
		{
			k = 0;
			while (curr_env->value[k])
			{
				dest[*index] = curr_env->value[k];
				(*index)++;
				k++;
			}
		}
		curr_env = curr_env->next;
	}
}

int	remove_quotes_in_arg(t_command *cmd, int n)
{
	char	*new_arg;

	if (!cmd || !cmd->args || !cmd->args[n])
		return (ERROR);
	new_arg = ft_calloc(ARG_MAX_SIZE, 1);
	if (!new_arg)
		return (ERROR);
	if (cpy_arg_without_quotes(&new_arg, cmd->args[n]) == ERROR)
		return (free(new_arg), ERROR);
	free(cmd->args[n]);
	cmd->args[n] = ft_strdup(new_arg);
	if (!cmd->args[n])
		return (free(new_arg), ERROR);
	return (free(new_arg), TRUE);
}

int	remove_surrounding_quotes(t_command *cmd)
{
	t_command	*current;
	int			i;

	current = cmd;
	while (current)
	{
		i = 0;
		while (current->args && current->args[i])
		{
			if (unclosed_quotes_case(current->args[i]))
				return (print_syntax_error(SYNTAX_ERROR_UNCLOSED_QUOTES),
					ERROR);
			if (are_quotes_in_arg(current->args[i]))
			{
				if (remove_quotes_in_arg(current, i) == ERROR)
					return (print_error_message("remove quotes error"),
						ERROR);
			}
			i++;
		}
		current = current->next;
	}
	return (TRUE);
}
