/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_comms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 19:15:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/08 11:45:37 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	check_commands(t_cmd *commands)
{
	int i;

	if (commands->cmd[0][0] == '.')
	{
		if (commands->cmd[0][1] == '.')
		{
			if (commands->cmd[0][2] == '/')
			{
				i = 0;
				while (commands->cmd[0][i])
					i++;
				i--;
				while (commands->cmd[0][i] != '/')
					i--;
				commands->path_cmd = commands->cmd[0];
				commands->cmd[0] = ft_strdup(&commands->cmd[0][i + 1]);
			}
		}
		else if (commands->cmd[0][1] == '/')
		{
			i = 0;
			while (commands->cmd[0][i])
				i++;
			i--;
			while (commands->cmd[0][i] != '/')
				i--;
			commands->path_cmd = commands->cmd[0];
			commands->cmd[0] = ft_strdup(&commands->cmd[0][i + 1]);
		}
	}

	if (commands->cmd[0][0] == '/')
	{
		i = 0;
		while (commands->cmd[0][i])
			i++;
		i--;
		while (commands->cmd[0][i] != '/')
			i--;
		commands->path_cmd = commands->cmd[0];
		commands->cmd[0] = ft_strdup(&commands->cmd[0][i + 1]);
	}
	return 0;
}

int	isntaletter(char t)
{
	if ((t >= 65 && t <= 90 ) || (t >= 97 && t <= 122) || (t >= 48 && t <= 57))
		return (1);
	return (0);
}

void	add_to(char *command, char *tested)
{
	int i = 0;
	char *yes;
	yes = malloc(ft_strlen(command) + ft_strlen(tested) + 1 * sizeof(char));
	while (command[i] != '$')
	{
		yes[i] = command[i];
		i++;
	}
	while (isntaletter(command[i]))
		i++;
	
}

void	is_a_variable(t_structenv *minienv)
{
	char	*test;
	char	*found;
	char	*tested;
	char	*test2;
	int		i;

	test = "ys $9HOMEd!";
	i = 1;
	found = strchr(test, '$');
	while (found[i])
	{
		if (isntaletter(found[i]) == 0)
			break ;
		i++;
	}
	test2 = ft_substr(found, 1, i - 1);
	tested = getenv(test2);
	if (tested == NULL)
	{
		while (minienv)
		{
			if (ft_strncmp(minienv->var), test2, ft_strlen(minienv->var) == 0)
				tested = minienv->invar;
		}
	}
	printf("%s\n", tested);
	add_to(command, tested, i);
}
