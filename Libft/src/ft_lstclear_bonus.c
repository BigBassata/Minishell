/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:27:45 by licohen           #+#    #+#             */
/*   Updated: 2024/12/12 17:27:46 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (lst != NULL)
	{
		while (*lst != NULL)
		{
			temp = (*lst)->next;
			del((*lst)->content);
			free(*lst);
			(*lst) = temp;
		}
	}
	*lst = NULL;
}
