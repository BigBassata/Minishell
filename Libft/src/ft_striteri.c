/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:30:29 by licohen           #+#    #+#             */
/*   Updated: 2024/12/12 17:30:30 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
void	ft_putchar_fd_bis(unsigned int i, char *c)
{
	while (i < 2)
	{
		write(1, &c[i], 1);
		i++;
	}
}

int	main(int ac, char **av)
{
	(void)ac;

	ft_striteri(av[1], ft_putchar_fd_bis);
	return (0);
}*/
