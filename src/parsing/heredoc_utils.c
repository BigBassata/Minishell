/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_count_char(char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s == c)
			count++;
		s++;
	}
	return (count);
}

void	cpy_refined_delimiter(char *delimiter, char *new_delimiter, char quote)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (delimiter[i])
	{
		if (delimiter[i] != quote)
		{
			new_delimiter[j] = delimiter[i];
			j++;
		}
		i++;
	}
	new_delimiter[j] = '\0';
}

int	cpy_and_expand_heredoc_arg(char *dest, char *src, int size,
	t_environment_var *env)
{
	char	*tmp_var;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (src[j] && i < size - 1)
	{
		if (is_env_var_format(&src[j]) && get_env_var_len(&src[j]) != 0)
		{
			tmp_var = get_env_var(&src[j]);
			if (!tmp_var)
				return (ERROR);
			cpy_expanded_var(dest, &i, tmp_var, env);
			j += get_env_var_len(&src[j]);
			free(tmp_var);
			continue ;
		}
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[size - 1] = '\0';
	return (TRUE);
}

int	check_heredoc_line(char *line, char *delimiter)
{
	if (!line || (line && ft_strcmp(delimiter, line) == 0)
		|| g_signal == 130)
		return (TRUE);
	else
		return (FALSE);
}
