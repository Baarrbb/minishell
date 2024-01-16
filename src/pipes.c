/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 16:30:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/16 17:39:38 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	close_all_pipes(int numPipes, int *pipefds)
{
	int	i;

	i = 0;
	while (i < 2 * numPipes)
	{
		close(pipefds[i]);
		i++;
	}
}

int	numbers_pipe(t_cmd *list)
{
	int	i;

	i = 0;
	if (list == NULL)
		return (0);
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

void	creation_pipes(t_exec *yipi)
{
	int	i;

	i = 0;
	while (yipi->numpipes > i)
	{
		if (pipe(yipi->pipefds + i * 2) < 0)
		{
			perror("couldn't pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}
