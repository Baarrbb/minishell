/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 18:33:59 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/17 19:36:42 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	return_free(char *tofree, int ret)
{
	free(tofree);
	return (ret);
}

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

// void	free_struct(t_cmd *cmd)
// {
// 	if (cmd)
// 	{
// 		if (cmd->path)
// 			free_char_tab(cmd->path);
// 		if (cmd->cmd)
// 			free_char_tab(cmd->cmd);
// 		free_redir(&(cmd->redir));
// 		free(cmd->path_cmd);
// 		free(cmd);
// 		cmd = 0;
// 	}
// }

void	free_struct(t_cmd **cmd)
{
	if (cmd)
	{
		if ((*cmd)->path)
			free_char_tab((*cmd)->path);
		if ((*cmd)->cmd)
			free_char_tab((*cmd)->cmd);
		free_redir(&((*cmd)->redir));
		free((*cmd)->path_cmd);
		free(*cmd);
		*cmd = 0;
	}
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
