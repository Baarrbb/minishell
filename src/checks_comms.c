/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_comms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 19:15:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/10 17:52:26 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	replace_commands(t_cmd *commandss)
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
	return (0);
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
	newstr[j] = '\0';
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
			if (ourenv[i][ft_strlen(tofind)] == '=')
				return (ourenv[i]);
		i++;
	}
	return (NULL);
}

void	is_a_variable(t_cmd *testons, char **env)
{
	int		j;
	char	**found;
	char	*tested;
	char	*test2;
	int i;
	int l;

	j = 0;
	i = 0;
	while (testons)
	{
		j = 0;
		while (testons->cmd[j])
		{
			l = 1;
			if (testons->quote_cmd[j] != 1)
			{
				found = ft_split(testons->cmd[j], '$');
				while (found[l])
				{
					i = 0;
					
					while (found[l][i])
					{
						if (isntaletter(found[l][i]) == 0)
							break ;
						i++;
					}
					test2 = ft_substr(found[l], 0, i);
					tested = getenv(test2);
					if (tested == NULL)
					{
						if (test2[0] == '?' && test2[1] == '\0')
						{
							char *exitval;
							exitval = ft_itoa(testons->exit_val);
							testons->cmd[j] = add_to(testons->cmd[j], exitval, ft_strlen(exitval));
							free(exitval);
						}
					}
					if (tested != NULL)
						testons->cmd[j] = add_to(testons->cmd[j], tested, ft_strlen(tested));
					free(test2);
					l++;
				}
				free_char_tab(found);
			}
			j++;
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
int	handle_quoting(t_cmd *quoting)
{
	int	i;
	int	j;

	while (quoting)
	{
		i = 0;
		j = 0;
		while(quoting->cmd[i])
			i++;
		//quoting->quote_cmd = malloc(i * sizeof(int));
		//if (quoting->quote_cmd == NULL)
		//	return (5);
		i = 0;
		while (quoting->cmd[i])
		{
			if (quoting->cmd[i][0] == '\'')
			{
				//quoting->cmd[i] = remove_quotes(quoting->cmd[i]);
				quoting->quote_cmd[i] = 1;
			}
			else if (quoting->cmd[i][0] == '\"')
			{
				//quoting->cmd[i] = remove_quotes(quoting->cmd[i]);
				quoting->quote_cmd[i] = 2;
			}
			else
				quoting->quote_cmd[i] = 0;
			i++;
		}
		quoting = quoting->next;
	}
}
