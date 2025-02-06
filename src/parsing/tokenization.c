/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tokenize_meta_chr(char **line, t_token **token_list)
{
	t_token	*token;

	token = NULL;
	if (**line == '<' && *(*line + 1) != '<')
		token = new_token("<", TYPE_REDIR_IN);
	else if (**line == '<' && *(*line + 1) == '<')
	{
		token = new_token("<<", TYPE_HEREDOC);
		(*line)++;
	}
	else if (**line == '>' && *(*line + 1) != '>')
		token = new_token(">", TYPE_REDIR_OUT);
	else if (**line == '>' && *(*line + 1) == '>')
	{
		token = new_token(">>", TYPE_REDIR_APPEND);
		(*line)++;
	}
	else if (**line == '|')
		token = new_token("|", TYPE_PIPE);
	(*line)++;
	if (token)
		return (add_last_token(token_list, token), TRUE);
	return (free_token_list(*token_list), ERROR);
}

int	tokenize_word(char **line, t_token **token_list)
{
	t_token	*token;
	int		word_len;
	char	*word;
	char	current_quote;

	token = NULL;
	word_len = 0;
	current_quote = 0;
	while (**line)
	{
		set_current_quote(**line, &current_quote);
		if (!current_quote && ft_strchr(" \t\n<>|", **line))
			break ;
		(*line)++;
		word_len++;
	}
	word = (char *)malloc((word_len + 1) * sizeof(char));
	if (!word)
		return (free_token_list(*token_list), ERROR);
	ft_strlcpy(word, (*line - word_len), word_len + 1);
	token = new_token(word, TYPE_WORD);
	free(word);
	if (token)
		return (add_last_token(token_list, token), TRUE);
	return (free_token_list(*token_list), ERROR);
}

t_token	*handle_tokenization(char *line)
{
	t_token	*token_list;

	token_list = NULL;
	while (*line)
	{
		if (*line == '<' || *line == '>' || *line == '|')
		{
			if (tokenize_meta_chr(&line, &token_list) == ERROR)
				return (print_error_message("malloc meta token error"), NULL);
		}
		else if (!ft_isspace(*line))
		{
			if (tokenize_word(&line, &token_list) == ERROR)
				return (print_error_message("malloc word token error"), NULL);
		}
		else
			line++;
	}
	return (token_list);
}

t_token	*tokenize_line(char *line)
{
	t_token	*token_list;

	line = filtered_line(line);
	if (!line)
		return (NULL);
	token_list = handle_tokenization(line);
	free(line);
	if (g_signal == 1)
		return (free_token_list(token_list), NULL);
	return (token_list);
}
