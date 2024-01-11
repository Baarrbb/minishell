/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:02:42 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/10 17:13:34 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	error_exec(t_cmd *comands)
{
	if (comands->path_cmd[ft_strlen(comands->cmd[0])] == '/')
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(comands->path_cmd, 2);
		ft_putstr_fd(": Is a directory", 2);
		exit(126);
	}
	else
	{
		printf("testerrorsfile\n");
		perror(comands->path_cmd);
	}
}

void	error_managing(t_cmd *command)
{
	struct stat	sb;

	if (stat(command->cmd[0], &sb) == -1 && ft_strchr(command->cmd[0], '/') != 0)
	{
		//nosuchfileordirectory error
		
		printf("test1\n");
		perror(command->cmd[0]);
		exit(127);
	}
	if (!command->path_cmd)
	{
		//command not found
		ft_putstr_fd(command->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	// 	perror("command not found error to do");
	error_exec(command);
}
