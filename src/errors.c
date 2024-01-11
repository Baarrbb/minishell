/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 14:02:42 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/11 13:48:06 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"


static void	no_cmd_slash(t_cmd *command)
{
	if (command->path_cmd == 0 && ft_strchr(command->cmd[0], '/') != 0)
	{		
		errno = ENOENT;
		perror(command->cmd[0]);
		exit(EXIT_FAILURE);
	}
}
static void	is_a_directory(t_cmd *command)
{
	if (command->path_cmd)
		{
			if (command->path_cmd[ft_strlen(command->path_cmd) - 1] == '/')
			{
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd(command->path_cmd, 2);
				ft_putstr_fd(": Is a directory\n", 2);
				free_list(&command);
				exit(126);
			}
		}
}
static void	no_such_file_or_directory(t_cmd *command)
{
	struct stat	sb;

	if (stat(command->cmd[0], &sb) == -1 && ft_strchr(command->cmd[0], '/') != 0)
	{
		printf("l39errors\n");
		perror(command->cmd[0]);
		free_list(&command);
		exit(127);
	}
}
static void	command_not_found(t_cmd *command)
{
	if (!command->path_cmd)
	{
		printf("l46errors\n");
		ft_putstr_fd(command->cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		free_list(&command);
		exit(127);
	}
}
void	error_managing(t_cmd *command)
{
	no_cmd_slash(command);
	is_a_directory(command);
	no_such_file_or_directory(command);
	command_not_found(command);
}
