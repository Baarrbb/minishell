/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_comms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 19:15:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/09 12:24:23 by ytouihar         ###   ########.fr       */
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

char	*add_to(char *command, char *envs, int w)
{
	int		i;
	int		j;
	int		l;
	char	*newstr;

	newstr = malloc(ft_strlen(command) + w + 1 * sizeof(char));
	if (newstr == NULL)
		return (0);
	i = 0;
	while (command[i] != '$')
	{
		newstr[i] = command[i];
		i++;
	}
	j = i;
	i = i + w;
	l = 0;
	while (envs[l])
		newstr[j++] = envs[l++];
	while (command[i])
		newstr[j++] = command[i++];
	newstr[j] = 0;
	free(command);
	return (newstr);
}

char *get_ourenv(char **ourenv, char* tofind)
{
	int	i;

	i = 0;
	while (ourenv[i])
	{
		if (ft_strncmp(ourenv[i], tofind, ft_strlen(tofind)))
			if (copy_env[i][ft_strlen(cmds[j])] == '=')
				return (copyenv[i]);
		i++;
	}
	return (NULL);
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
	while (testons)
	{
		j = 0;
		while (testons->cmd[j])
		{
			l = 0;
			i = 0;
			if (testons->quote_cmd[j] != 1)
			{
				found = ft_split(testons->cmd[j], '$');
				while (found[l])
				{
					while (found[l][i])
					{
						if (isntaletter(found[l][i]) == 0)
							break ;
						i++;
					}
					test2 = ft_substr(found[l], 0, i);
					tested = get_ourenv(test2);
					if (tested != NULL)
						testons->cmd[j] = add_to(testons->cmd[j], tested, ft_strlen(tested));
					free(test2);
					l++;
				}
				free_char_tab(found);
				j++;
			}
		}
		testons = testons->next;
	}
}

char	*remove_quotes(char *str)
{
	char	*newstr;
	int		i;

	i = 0;
	newstr = malloc((ft_strlen(str) - 1) * sizeof(char));
	if (newstr == NULL)
		return (0);
	while (str[i + 2])
	{
		newstr[i] = str[i + 1];	
		i++;
	}
	free(str);
	newstr[i] = '\0';
	return (newstr);
}
void	handle_quoting(t_cmd *quoting)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	quoting->
	quoting->quote_cmd = malloc(quoting->cmd * sizeof(int));
	while (quoting->cmd[i])
	{
		if (quoting->cmd[i][0] == '\'')
		{
			quoting->quote_cmd[i] = remove_quotes(quoting->quote_cmd[i]);
			quoting->quote_cmd[i] = 1;
		}
		else if (quoting->cmd[i][0] == '\"')
		{
			quoting->quote_cmd[i] = remove_quotes(quoting->quote_cmd[i]);
			quoting->quote_cmd[i] = 2;
		}
		else
			quoting->quote_cmd[i] = 0;
		i++;
	}
}
