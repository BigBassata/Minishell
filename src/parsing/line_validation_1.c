/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_validation_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_null_line(char *line)
{
	if (!line)
		return (ft_putendl_fd("exit minishell", STDOUT_FILENO), TRUE);
	return (FALSE);
}

int	is_void_case(char *line)
{
	int	len;

	g_signal = 0;
	len = ft_strlen(line);
	if (len == 0)
		return (free(line), TRUE);
	if (len == 1 && line[0] == '\n')
		return (free(line), TRUE);
	return (FALSE);
}

int	is_ignored_case(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len == 0)
		return (TRUE);
	if (len == 1 && (line[0] == ':' || line[0] == '!'))
		return (TRUE);
	return (FALSE);
}

int	is_syntax_error(char *line)
{
	char	message[3];

	if (unclosed_quotes_case(line))
		return (print_syntax_error(SYNTAX_ERROR_UNCLOSED_QUOTES), TRUE);
	if (not_handled_case(line, message))
		return (print_syntax_error_message(message), TRUE);
	if (bad_redirection_syntax(line))
		return (print_syntax_error(SYNTAX_ERROR_REDIRECTION), TRUE);
	if (bad_pipe_syntax(line))
		return (print_syntax_error(SYNTAX_ERROR_PIPE), TRUE);
	return (FALSE);
}

char	*filtered_line(char *line)
{
	char	*new_line;

	new_line = ft_strtrim(line, " \t\n\v\f\r");
	free(line);
	if (!new_line)
		return (NULL);
	if (is_ignored_case(new_line))
		return (free(new_line), NULL);
	if (is_syntax_error(new_line))
		return (free(new_line), NULL);
	return (new_line);
}
