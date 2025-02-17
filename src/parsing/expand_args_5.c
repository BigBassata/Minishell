/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_cpy_expanded_var(char **dest, char **src,
	t_environment_var *env, int *len)
{
	char	*tmp_var;
	int		old_len_0;
	int		var_value_len;

	tmp_var = get_env_var(*src);
	if (!tmp_var)
		return (ERROR);
	if (get_env_var_len(tmp_var) == 0)
		return (free(tmp_var), ERROR);
	var_value_len = len[1] + get_env_var_value_len(tmp_var,
		get_env_var_len(tmp_var), env);
	while (var_value_len >= len[0] - 1)
	{
		old_len_0 = len[0];
		if (resize_and_copy_expandable_dest(dest, &len[0]) == ERROR)
			return (free(tmp_var), ERROR);
		if (old_len_0 == len[0])
			return (free(tmp_var), ERROR);
		var_value_len = len[1] + get_env_var_value_len(tmp_var,
			get_env_var_len(tmp_var), env);
	}	
	cpy_expanded_var(*dest, &len[1], tmp_var, env);
	*src += get_env_var_len(tmp_var);
	return (free(tmp_var), TRUE);
}
