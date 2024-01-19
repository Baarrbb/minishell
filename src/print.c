/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:08:30 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 15:56:21 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redir(t_redir *redir)
{
	if (redir)
	{
		while (redir)
		{
			printf("out %d end %d\n", redir->out, redir->out_end);
			printf("in %d read %d\n", redir->in, redir->in_read);
			printf("Filename .%s.\n", redir->filename);
			redir = redir->next;
		}
	}
}

void	print_struct(t_cmd *cmd)
{
	int	i;

	if (!cmd)
	{
		printf("NULL\n");
		return ;
	}
	if (cmd->cmd)
	{
		i = -1;
		while (cmd->cmd[++i])
		{
			if (i == 0)
				printf("\ncmd : .%s.\n", cmd->cmd[i]);
			else
				printf("args : .%s.\n", cmd->cmd[i]);
		}
	}
	else
		printf("\ncmd->cmd : (null)\n");
	printf("Is that a builtin : %d\n", cmd->builtin);
	printf("Cmd path : %s\n", cmd->path_cmd);
	if (cmd->redir)
		print_redir(cmd->redir);
}

void	print_linked_list(t_cmd *pipe)
{
	while (pipe)
	{
		print_struct(pipe);
		pipe = pipe->next;
	}
}
