/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 23:21:20 by barb              #+#    #+#             */
/*   Updated: 2023/11/21 15:12:23 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len || !len)
	{
		ret = (char *)ft_calloc(1, sizeof(char));
		if (!ret)
			return (NULL);
		return (ret);
	}
	if (len >= s_len)
		len = s_len - start;
	else if (len > s_len - start)
		len--;
	ret = (char *)ft_calloc((len + 1), sizeof(char));
	if (!ret)
		return (NULL);
	s += start + (len - 1);
	ret += len;
	while (len--)
		*--ret = *s--;
	return (ret);
}
