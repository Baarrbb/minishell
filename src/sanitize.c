/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:33:59 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/04 19:53:01 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_char_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

static void	free_redir(t_redir **redir)
{
	t_redir	*tmp;

	while (*redir)
	{
		tmp = (*redir)->next;
		free((*redir)->filename);
		free(*redir);
		*redir = tmp;
	}
	*redir = 0;
}

void	free_list(t_cmd **list)
{
	t_cmd	*tmp;

	if (list)
	{
		while (*list)
		{
			if ((*list)->path)
				free_char_tab((*list)->path);
			if ((*list)->cmd)
				free_char_tab((*list)->cmd);
			free_redir(&((*list)->redir));
			free((*list)->path_cmd);
			tmp = (*list)->next;
			free(*list);
			*list = tmp;
		}
		*list = 0;
	}
}
