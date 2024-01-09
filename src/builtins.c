/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:37:27 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/09 11:32:52 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	good_flag(char *arguments)
{
	int	i;

	i = 1;
	if (arguments[0] != '-')
		return (1);
	while (arguments[i])
	{
		if (arguments[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

void	echo(char **cmds_args)
{
	int	next_line;
	int	i;

	i = 1;
	next_line = 0;
	while (good_flag(cmds_args[i]) == 0)
	{
		next_line = 1;
		i++;
	}
	while (cmds_args[i])
	{
		printf("%s", cmds_args[i]);
		i++;
	}
	if (next_line == 1)
		printf("\n");
}

void	pwd()
{
	char cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		printf("getcwd() error");
}

void remove_elem(char ***arr, int index)
{
	int i;

	i = index;
	while ((*arr)[i] != NULL)
	{
		(*arr)[i] = (*arr)[i+1];
		i++;
	}
	(*arr)[i] = NULL;
}

void	unset(char **cmds, char **copy_env)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (copy_env[i])
	{
		j = 1;
		while (cmds[j])
		{
			if (ft_strncmp(copy_env[i], cmds[j], ft_strlen(cmds[2])) == 0)
				if (copy_env[i][ft_strlen(cmds[j])] == '=')
					remove_elem(&copy_env, i);
			j++
		}
		i++;
	}
	i = 0;
	while (copy_env[i])
	{
		printf("%s\n", copy_env[i]);
		i++;
	}
}

void	exit()
{
	free_char_tab(command);
	free_list(&pipe);
	exit(1);
}