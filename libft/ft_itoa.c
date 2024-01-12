/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 23:19:03 by barb              #+#    #+#             */
/*   Updated: 2023/11/21 15:10:24 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	len_malloc(long n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		len++;
		n *= -1;
	}
	while (n > 9)
	{
		len++;
		n /= 10;
	}
	len++;
	return (len);
}

char	*ft_itoa(int n)
{
	int		len;
	long	ln;
	char	*nb;

	ln = n;
	len = len_malloc(ln);
	nb = (char *)malloc((len + 1) * sizeof(char));
	if (!nb)
		return (NULL);
	if (ln < 0)
	{
		nb[0] = '-';
		ln *= -1;
	}
	nb[len] = '\0';
	while (ln > 9)
	{
		nb[--len] = (ln % 10) + 48;
		ln /= 10;
	}
	nb[--len] = (ln % 10) + 48;
	return (nb);
}
