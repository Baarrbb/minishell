/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:33:59 by bsuc              #+#    #+#             */
/*   Updated: 2023/12/30 19:00:18 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_char_tab(char **tab)
{
	int	i;

	i = -1;
	if (tab[0])
	{
		while (tab[++i])
			free(tab[i]);
		free(tab);
	}
}

void	free_list(t_cmd **list)
{
	t_cmd	*tmp;

	while(*list)
	{
		tmp = (*list)->next;
		free(*list);
		*list = tmp;
	}
	*list = 0;
}
