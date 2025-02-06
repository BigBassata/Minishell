/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args_4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	resize_and_copy_expandable_dest(char **dest, int *dest_size)
{
	char	*temp;
	int		new_size;

	if (!dest || !*dest)
		return (print_error_message("error !dest || !*dest"), ERROR);
	if (*dest_size > INT_MAX / 2)
		return (print_error_message("error *dest_size > INT_MAX / 2"), ERROR);
	new_size = *dest_size + ARG_MAX_SIZE;
	temp = (char *)ft_calloc(new_size, 1);
	if (!temp)
		return (print_error_message("error !temp"), ERROR);
	ft_strcpy(temp, *dest);
	free(*dest);
	*dest = temp;
	*dest_size = new_size;
	return (TRUE);
}

int	resize_and_copy_dest(char **dest)
{
	char	*temp;
	int		new_size;

	if (!dest || !*dest)
		return (print_error_message("error !dest || !*dest"), ERROR);
	new_size = ft_strlen(*dest);
	if (new_size > INT_MAX / 2)
		return (print_error_message("error new_size > INT_MAX / 2"), ERROR);
	new_size += ARG_MAX_SIZE;
	temp = (char *)ft_calloc(new_size, 1);
	if (!temp)
		return (print_error_message("error !temp"), ERROR);
	ft_strcpy(temp, *dest);
	free(*dest);
	*dest = temp;
	return (new_size);
}

void	toggle_in_single_quotes(char c, int *in_squotes)
{
	if (c == '\'' && !*in_squotes)
		*in_squotes = 1;
	else if (c == '\'' && *in_squotes)
		*in_squotes = 0;
}

int	are_quotes_in_arg(char *arg)
{
	char	current_quote;

	current_quote = 0;
	while (arg && *arg)
	{
		if ((*arg == '"' || *arg == '\'') && !current_quote)
			set_current_quote(*arg, &current_quote);
		else if (current_quote && *arg == current_quote)
			return (TRUE);
		arg++;
	}
	return (FALSE);
}

int	cpy_arg_without_quotes(char **dst, char *src)
{
	char	current_quote;
	int		dst_len;
	int		j;

	current_quote = 0;
	dst_len = ARG_MAX_SIZE;
	j = 0;
	while (src && *src && j < dst_len - 1)
	{
		if ((*src == '"' || *src == '\'') && !current_quote)
			set_current_quote(*src, &current_quote);
		else if (current_quote && *src == current_quote)
			current_quote = 0;
		else
			(*dst)[j++] = *src;
		src++;
		if (j == dst_len - 1)
			dst_len = resize_and_copy_dest(dst);
		if (dst_len == ERROR)
			return (print_error_message("error resize_and_copy_dest"), ERROR);
	}
	(*dst)[j] = '\0';
	return (TRUE);
}
