/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (new_token)
	{
		new_token->value = ft_strdup(value);
		if (!new_token->value)
			return (free(new_token), NULL);
		new_token->type = type;
		new_token->next = NULL;
	}
	return (new_token);
}

void	add_last_token(t_token **first_token, t_token *new_token)
{
	t_token	*current;

	if (!*first_token)
		*first_token = new_token;
	else
	{
		current = *first_token;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
}

void	free_token_list(t_token *first_token)
{
	t_token	*tmp;

	while (first_token)
	{
		tmp = first_token;
		first_token = first_token->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}
