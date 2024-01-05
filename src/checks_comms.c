/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_comms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 19:15:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/05 14:59:57 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_commands(t_cmd *commands)
{
	t_redir	*test;
	t_redir	*tmp;
	t_cmd	*yes;
	
	while (yes)
	{
		if (!yes->path_cmd && !yes->cmd && !yes->path)
		{
			if (yes->redir != NULL)
			{
				test = yes->redir;
				tmp = yes->redir;
			}
			yes = yes->next;
		}

	}
	return 0;
}