/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 16:54:00 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/16 17:57:03 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*get_ourenv(char *tofind, char **ourenv, char *sortie)
{
	int	i;

	i = 0;
	if (ourenv)
	{
		while (ourenv[i])
		{
			if (ft_strncmp(ourenv[i], tofind, ft_strlen(tofind)) == 0)
				return (ft_strdup(&ourenv[i][ft_strlen(tofind) + 1]));
			i++;
		}
	}
	if (ft_strncmp(tofind, "?=\0", ft_strlen(tofind) + 1) == 0)
		return (sortie);
	return (NULL);
}

int	in_quotes(char test, int etat)
{
	if (test == '\'' && etat == -1)
		etat = 2;
	else if (test == '\"' && etat == -1)
		etat = 1;
	else if (test == '\'' && etat == 2)
		etat = -1;
	else if (test == '\"' && etat == 1)
		etat = -1;
	return (etat);
}
