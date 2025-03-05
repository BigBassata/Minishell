/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol_base_10_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:29:40 by licohen           #+#    #+#             */
/*   Updated: 2024/03/05 17:29:41 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	compute_limits(int negative, unsigned long *cutoff, int *cutlim)
{
	unsigned long	limit;

	if (negative)
		limit = -(unsigned long)LONG_MIN;
	else
		limit = LONG_MAX;
	*cutoff = limit / 10;
	*cutlim = (int)(limit % 10);
}

static int	will_overflow(unsigned long acc, int digit, unsigned long cutoff,
	int cutlim)
{
	if (acc > cutoff)
		return (1);
	if (acc == cutoff)
	{
		if (digit > cutlim)
			return (1);
	}
	return (0);
}

static unsigned long	handle_overflow(int negative)
{
	if (negative)
		return (-(unsigned long)LONG_MIN);
	else
		return (LONG_MAX);
}

const char	*ft_accumulate_number(const char *s, int negative,
	unsigned long *acc, int *overflow)
{
	unsigned long	cutoff; 
	int				cutlim;
	int				digit;

	cutoff = 0;
	cutlim = 0;
	digit = 0;
	compute_limits(negative, &cutoff, &cutlim);
	while (*s != '\0' && *s >= '0' && *s <= '9')
	{
		digit = *s - '0' ;
		if (*overflow == 0)
		{
			if (will_overflow(*acc, digit, cutoff, cutlim))
			{
				*overflow = 1 ;
				*acc = handle_overflow(negative) ;
			}
			else
				*acc = *acc * 10 + digit;
		}
		s++ ;
	}
	return (s);
}
