/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:15:17 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 17:16:32 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_redir(t_cmd **cmd, char **args, int type)
{
	t_redir	*new;
	t_redir	*redir;

	redir = (*cmd)->redir;
	new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return ;
	ft_memset(new, 0, sizeof(t_redir));
	if (type == 1)
		new->out = 1;
	else if (type == 2)
		new->out_end = 1;
	else if (type == 3)
		new->in = 1;
	else if (type == 4)
		new->in_read = 1;
	new->filename = ft_strdup(args[1]);
	ft_lstadd_back(&redir, new);
	(*cmd)->redir = redir;
}

int	is_redir(t_cmd **cmd, char **args, int i)
{
	if (!ft_strncmp(args[i], ">", ft_strlen(args[i])))
	{
		init_redir(cmd, &args[i], 1);
		return (2);
	}
	else if (!ft_strncmp(args[i], ">>", ft_strlen(args[i])))
	{
		init_redir(cmd, &args[i], 2);
		return (2);
	}
	else if (!ft_strncmp(args[i], "<", ft_strlen(args[i])))
	{
		init_redir(cmd, &args[i], 3);
		return (2);
	}
	else if (!ft_strncmp(args[i], "<<", ft_strlen(args[i])))
	{
		init_redir(cmd, &args[i], 4);
		return (2);
	}
	return (0);
}
