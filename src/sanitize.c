/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:33:59 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 18:02:52 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	free_tab_and_list(char **tab, t_cmd **lst)
// {
// 	free_char_tab(tab);
// 	free_list(lst);
// 	
// }

void	free_char_tab(char **tab)
{
	int	i;

	i = -1;
	if (*tab)
	{
		while (tab[++i])
			free(tab[i]);
	}
	if (tab)
		free(tab);
}

static void	free_redir(t_redir **redir)
{
	t_redir	*tmp;

	if (redir)
	{
		while (*redir)
		{
			tmp = (*redir)->next;
			free((*redir)->filename);
			free(*redir);
			*redir = tmp;
		}
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
