/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:26:27 by licohen           #+#    #+#             */
/*   Updated: 2024/12/12 17:26:28 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*str;

	if (nmemb == 0 && size == 0 && (nmemb >= SIZE_MAX / size))
		return (NULL);
	str = (void *) malloc(size * nmemb);
	if (!str)
		return (NULL);
	ft_bzero(str, nmemb * size);
	return (str);
}
/*
#include <stdio.h>

int	main(void)
{
	char	*str;

	str = (char *) ft_calloc(10, 1);
	printf("%s\n", str);
	return (0);
}*/
