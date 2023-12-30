/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 23:21:17 by barb              #+#    #+#             */
/*   Updated: 2023/12/30 21:57:08 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ischarset(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ncharset(char const *s1, char const *set)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (ischarset(s1[i], (char *)set) && s1[i])
	{
		count++;
		i++;
	}
	if (!s1[i])
		return (count);
	i = ft_strlen(s1) - 1;
	while (ischarset(s1[i], (char *)set) && i != 0)
	{
		count++;
		i--;
	}
	return (count);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	int		i;
	int		len;
	char	*s;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	len = ft_strlen(s1) - ncharset(s1, set);
	while (ischarset((char)s1[i], (char *)set))
		i++;
	s = ft_substr(s1, i, len);
	return (s);
}
