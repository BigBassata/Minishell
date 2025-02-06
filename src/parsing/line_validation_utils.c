/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_validation_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julombar <julombar@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 10:05:09 by julombar          #+#    #+#             */
/*   Updated: 2024/12/01 12:29:35 by julombar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_current_quote(char c, char *current_quote)
{
	if (c == '"' || c == '\'')
	{
		if (!*current_quote)
			*current_quote = c;
		else if (*current_quote == c)
			*current_quote = 0;
	}
}
