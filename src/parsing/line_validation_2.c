/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_validation_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unclosed_quotes_case(char *line)
{
	char	current_quote;

	current_quote = 0;
	while (*line)
	{
		set_current_quote(*line, &current_quote);
		line++;
	}
	if (!current_quote)
		return (FALSE);
	return (TRUE);
}

int	not_handled_case(char *line, char *message)
{
	char	current_quote;

	current_quote = 0;
	if (!message)
		return (ft_putendl_fd("Error: NULL ptr not_handled_case()", 2), FALSE);
	while (*line)
	{
		set_current_quote(*line, &current_quote);
		if (!current_quote && *line == ';')
			return (ft_strcpy(message, ";"), TRUE);
		else if (!current_quote && *line == '&' && *(line + 1) != '&')
			return (ft_strcpy(message, "&"), TRUE);
		else if (!current_quote && *line == '&' && *(line + 1) == '&')
			return (ft_strcpy(message, "&&"), TRUE);
		else if (!current_quote && *line == '|' && *(line + 1) == '|')
			return (ft_strcpy(message, "||"), TRUE);
		line++;
	}
	return (FALSE);
}

int	bad_redirection_syntax(char *line)
{
	char	current_quote;
	char	current_redirection;

	current_quote = 0;
	current_redirection = 0;
	while (*line)
	{
		set_current_quote(*line, &current_quote);
		if (!current_quote && (*line == '<' || *line == '>'))
		{
			current_redirection = *line;
			line++;
			if (*line == current_redirection)
				line++;
			while (*line == ' ' || *line == '\t')
				line++;
			if (*line == '<' || *line == '>' || *line == '|' || *line == '\0')
				return (TRUE);
		}
		line++;
	}
	return (FALSE);
}

int	bad_pipe_syntax(char *line)
{
	char	current_quote;
	int		pipe_detected;

	current_quote = 0;
	pipe_detected = 0;
	if (*line == '|')
		return (TRUE);
	while (*line)
	{
		set_current_quote(*line, &current_quote);
		if (*line == '|' && !current_quote)
		{
			if (pipe_detected || *(line + 1) == '\0')
				return (TRUE);
			pipe_detected = 1;
		}
		if (pipe_detected && !ft_isspace(*line) && *line != '|')
			pipe_detected = 0;
		line++;
	}
	if (pipe_detected)
		return (TRUE);
	return (FALSE);
}
