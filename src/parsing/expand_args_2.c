/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_var_format(char *arg)
{
	char	next;

	if (arg && *arg == '$')
	{
		next = *(arg + 1);
		if (next == '?' || ft_isalpha(next) || next == '_')
			return (TRUE);
	}
	return (FALSE);
}

int	get_env_var_len(char *arg)
{
	int	len;

	if (!arg || !arg[0] || arg[0] != '$' || !arg[1])
		return (0);
	if (arg[1] == '?')
		return (2);
	if (!ft_isalpha(arg[1]) && arg[1] != '_')
		return (0);
	len = 1;
	while (arg[len] && (ft_isalnum(arg[len]) || arg[len] == '_'))
		len++;
	return (len);
}

int	get_env_var_value_len(char *env_var, int var_len, t_environment_var *env)
{
	int					env_var_len;
	char				*env_var_name;
	t_environment_var	*curr_env;

	if (!env || !env_var)
		return (0);
	env_var_name = &env_var[1];
	env_var_len = var_len - 1;
	curr_env = env;
	if (env_var_name[0] == '?' && env_var_name[1] == '\0')
		return (last_status_len(env->last_exit_code));
	while (curr_env)
	{
		if (ft_strncmp(curr_env->key, env_var_name, env_var_len) == 0
			&& curr_env->key[env_var_len] == '\0' && curr_env->value)
			return (ft_strlen(curr_env->value));
		curr_env = curr_env->next;
	}
	return (0);
}

char	*get_env_var(char *arg)
{
	char	*env_var;
	int		env_var_len;

	env_var = NULL;
	env_var_len = get_env_var_len(arg);
	if (env_var_len == 0)
		return (NULL);
	env_var = (char *)malloc((env_var_len + 1) * sizeof(char));
	if (!env_var)
		return (NULL);
	ft_strlcpy(env_var, arg, env_var_len + 1);
	return (env_var);
}

int	cpy_and_expand_arg(char **dest, char *src, t_environment_var *env)
{
	int		in_squotes;
	int		len[2];

	in_squotes = 0;
	len[0] = ARG_MAX_SIZE;
	len[1] = 0;
	while (*src && len[1] < len[0] - 1)
	{
		toggle_in_single_quotes(*src, &in_squotes);
		if (!in_squotes && is_env_var_format(src) && get_env_var_len(src) != 0)
		{
			if (handle_cpy_expanded_var(dest, &src, env, len)
				== ERROR)
				return (ERROR);
			continue ;
		}
		(*dest)[len[1]++] = *src++;
		if (len[1] == len[0] - 1
			&& resize_and_copy_expandable_dest(dest, &len[0]) == ERROR)
			return (ERROR);
	}
	(*dest)[len[1]] = '\0';
	return (TRUE);
}
