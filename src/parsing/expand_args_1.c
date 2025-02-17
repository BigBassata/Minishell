/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	vars_values_lens(char *arg, t_environment_var *env, int *vars_len,
	int *values_len)
{
	char	*temp_var;
	int		env_var_len;
	int		i;

	i = 0;
	while (arg[i])
	{
		if (is_env_var_format(&arg[i]) && get_env_var_len(&arg[i]) != 0)
		{
			env_var_len = get_env_var_len(&arg[i]);
			temp_var = (char *)malloc((env_var_len + 1) * sizeof(char));
			if (!temp_var)
				return (ERROR);
			ft_strlcpy(temp_var, &arg[i], env_var_len + 1);
			*vars_len += env_var_len;
			*values_len += get_env_var_value_len(temp_var, env_var_len, env);
			free(temp_var);
			i += env_var_len;
		}
		else
			i++;
	}
	return (TRUE);
}

int	expand_vars_in_arg(t_command *cmd, t_environment_var *env, int n)
{
	char	*new_arg;

	new_arg = ft_calloc(ARG_MAX_SIZE, 1);
	if (!new_arg)
		return (print_error_message("expand var in arg error"), ERROR);
	if (cpy_and_expand_arg(&new_arg, cmd->args[n], env) == ERROR)
	{
		free(new_arg);
		return (print_error_message("cpy_and_update_arg() error"), ERROR);
	}
	free(cmd->args[n]);
	cmd->args[n] = new_arg;
	return (TRUE);
}

int	is_expandable_var(char *arg)
{
	char	next;

	while (*arg)
	{
		next = *(arg + 1);
		if (*arg == '$' && next == '?')
			return (TRUE);
		else if (*arg == '$' && (ft_isalpha(next) || next == '_'))
			return (TRUE);
		arg++;
	}
	return (FALSE);
}

int	handle_expand_args(t_command *cmd, t_environment_var *env)
{
	t_command	*current;
	int			i;

	current = cmd;
	while (current)
	{
		if (current->args && current->args[0])
		{
			i = 0;
			while (current->args[i])
			{
				if (is_expandable_var(current->args[i]))
				{
					if (expand_vars_in_arg(current, env, i) == ERROR)
						return (ERROR);
				}
				i++;
			}
		}
		current = current->next;
	}
	return (TRUE);
}

t_command	*expand_args(t_command *cmd, t_environment_var *env)
{
	if (!cmd)
		return (NULL);
	if (handle_expand_args(cmd, env) == ERROR)
		return (free_cmd_list(cmd), NULL);
	if (remove_surrounding_quotes(cmd) == ERROR)
		return (free_cmd_list(cmd), NULL);
	return (cmd);
}
