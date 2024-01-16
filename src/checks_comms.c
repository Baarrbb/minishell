/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_comms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 19:15:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/16 16:55:40 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	replace_commands(t_cmd *commandss)
{
	int	i;

	i = 0;
	while (commandss->cmd[0][i])
		i++;
	i--;
	while (commandss->cmd[0][i] != '/')
		i--;
	commandss->path_cmd = commandss->cmd[0];
	commandss->cmd[0] = ft_strdup(&commandss->cmd[0][i + 1]);
}

int	check_commands(t_cmd *commands)
{
	if (commands->cmd)
	{
		if (commands->cmd[0][0] == '.')
		{
			if (commands->cmd[0][1] == '.')
			{
				if (commands->cmd[0][2] == '/')
					replace_commands(commands);
			}
			else if (commands->cmd[0][1] == '/')
				replace_commands(commands);
		}
		if (commands->cmd[0][0] == '/')
			replace_commands(commands);
	}
	return (0);
}
