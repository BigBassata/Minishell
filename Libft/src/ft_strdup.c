/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:30:25 by licohen           #+#    #+#             */
/*   Updated: 2024/12/12 17:30:26 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	char	*src;

	src = (char *)s;
	dest = NULL;
	dest = (char *) malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!dest)
		return (NULL);
	dest = ft_strcpy(dest, src);
	return (dest);
}

/*#include <stdio.h>

int	main(int ac, char **av)
{
	char	tab[] = "Hello";
	char	*dest;

	(void)ac;
	(void)av;
	dest = ft_strdup(tab);
	printf("%s\n", dest);
	return (0);
}*/
