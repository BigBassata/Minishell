/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol_base_10.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: licohen <licohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:29:40 by licohen           #+#    #+#             */
/*   Updated: 2024/03/05 17:29:41 by licohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static const char	*skip_whitespace(const char *s)
{
	while (ft_isspace((unsigned char)*s))
		s++;
	return (s);
}

static int	parse_sign(const char **s_ptr)
{
	int	negative;

	negative = 0;
	if (**s_ptr == '-')
	{
		negative = 1;
		*s_ptr = *s_ptr + 1;
	}
	else if (**s_ptr == '+')
		*s_ptr = *s_ptr + 1;
	return (negative);
}

static const char	*process_conversion(const char *s, int neg, long *result,
	int *overflow)
{
	unsigned long	acc;

	acc = 0;
	s = ft_accumulate_number(s, neg, &acc, overflow);
	if (*overflow == 0)
	{
		if (neg)
			*result = -(long)acc;
		else
			*result = (long)acc;
	}
	else
		*result = (long)acc;
	return (s);
}

long	ft_strtol_base_10(const char *nptr, char **endptr)
{
	const char	*s;
	int			neg;
	long		result;
	int			overflow;

	s = nptr;
	neg = 0;
	result = 0;
	overflow = 0;
	s = skip_whitespace(s);
	neg = parse_sign(&s);
	s = process_conversion(s, neg, &result, &overflow);
	if (endptr)
		*endptr = (char *)s;
	return (result);
}
