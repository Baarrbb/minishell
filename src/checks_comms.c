/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_comms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 19:15:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/08 15:24:42 by ytouihar         ###   ########.fr       */
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

char	*add_to(char *command, char *tested, int w)
{
	int		i;
	int		j;
	int		l;
	char	*yes;

	yes = malloc(ft_strlen(command) + w + 1 * sizeof(char));
	i = 0;
	while (command[i] != '$')
	{
		yes[i] = command[i];
		i++;
	}
	j = i;
	i = i + w;
	l = 0;
	while (tested[l])
		yes[j++] = tested[l++];
	while (command[i])
		yes[j++] = command[i++];
	yes[j] = 0;
	free(command);
	return (yes);
}

void	is_a_variable(t_cmd *testons)
{
	char	**found;
	char	*tested;
	char	*test2;
	int		i;

	i = 0;
	int j = 0;
	int l = 0;
	while (testons->cmd[j])
	{
		l = 0;
		i = 0;
		found = ft_split(testons->cmd[j], '$');
		while (found[l])
		{
			while (found[l][i] && isntaletter(found[l][i]) == 1)
				i++;
			test2 = ft_substr(found[l], 0, i);
			tested = getenv(test2);
			if (getenv(test2) != NULL)
				testons->cmd[j] = add_to(testons->cmd[j], getenv(test2), ft_strlen(tested));
			free(test2);
			l++;
		}
		j++;
	}
}
