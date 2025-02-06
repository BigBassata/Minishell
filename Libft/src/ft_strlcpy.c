/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:30:08 by licohen           #+#    #+#             */
/*   Updated: 2024/12/12 17:30:08 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(src);
	if (siz != 0)
	{
		while (src[i] && i < siz - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (len);
}
/*
int	main(int ac, char **av)
{
	(void)ac;
	int	i;

	i = ft_strlcpy(av[1], av[2], 3);
	printf("%d\n", i);
	return (0);
}
*/
