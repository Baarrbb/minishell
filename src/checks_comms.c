/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_comms.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 19:15:32 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/15 19:54:26 by ytouihar         ###   ########.fr       */
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

int	isntaletter(char t)
{
	if ((t >= 65 && t <= 90 ) || (t >= 97 && t <= 122) || (t >= 48 && t <= 57) || t == '_')
		return (1);
	return (0);
}

char	*add_to(char *command, char *result, int ex_len, int len)
{
	int		index_cmd;
	int		index_newstr;
	int		index_result;
	char	*newstr;

	index_result = 0;
	newstr = malloc((ft_strlen(command) - ex_len + len + 1) * sizeof(char));
	if (newstr == NULL)
		return (0);
	index_cmd = 0;
	while (command[index_cmd] != '$')
	{
		newstr[index_cmd] = command[index_cmd];
		index_cmd++;
	}
	index_newstr = index_cmd;
	index_cmd = index_cmd + ex_len;
	while (result[index_result])
		newstr[index_newstr++] = result[index_result++];
	while (command[index_cmd])
		newstr[index_newstr++] = command[index_cmd++];
	newstr[index_newstr] = '\0';
	free(command);
	return (newstr);
}

char *get_ourenv(char* tofind, char **ourenv, char *sortie)
{
	int	i;

	i = 0;
	if (ourenv)                                                                                                                                         
	{
		while (ourenv[i])
		{
			if (ft_strncmp(ourenv[i], tofind, ft_strlen(tofind)) == 0)
				if (ourenv[i][ft_strlen(tofind)] == '=')
					return (ft_strdup(&ourenv[i][ft_strlen(tofind) + 1]));
			i++;
		}
	}
	//if (tofind)
		//return (sortie);
	return (NULL);
}

int	searching_etat(char test, int etat)
{
	if (test == '\'' && etat == -1)
		etat = 2;
	else if (test == '\"' && etat == -1)
		etat = 1;
	else if (test == '\'' && etat == 2)
		etat = -1;
	else if (test == '\"' && etat == 1)
		etat = -1;
	return (etat);
}

static char	*remove_dollar(char *str)
{
	char	*newstr;
	int		index_cmd;
	int		index_str;

	index_cmd = 0;
	index_str = 0;
	newstr = malloc((ft_strlen(str)) * sizeof(char));
	while (str[index_cmd] != '$')
	{
		newstr[index_cmd] = str[index_cmd];
		index_cmd++;

	}
	index_str = index_cmd;
	index_cmd++;
	while (str[index_cmd])
	{
		newstr[index_str] = str[index_cmd];
		index_cmd++;
		index_str++;
	}
	newstr[index_str] = 0;
	return (newstr);	
}

char	*sub_strs(char *str, int pos, char **our_envp, char *sortie)
{
	int		i;
	char	*variable;
	char	*result;

	i = pos;
	if (str[i + 1] == '?')
		i = i + 2;
	else
	{
		while (str[++i])
		{
			if (isntaletter(str[i]) == 0)
				break ;
		}
	}
	variable = ft_substr(str, (pos + 1), (i - pos - 1)); 
	variable = strjoin(variable, "=");
	result = get_ourenv(variable, our_envp, sortie);
	if (result != NULL)
		str = add_to(str, result, (i - pos), ft_strlen(result));
	else
		str = remove_dollar(str);
	free(variable);
	return (str); // error malloc bon code return 1 || // error malloc false code return 0
}

int	etat_mort(t_cmd *cmd, char **our_envp, char *sortie)
{
	int i;
	int	etat;
	int	j;

	i = 0;
	j = 0;
	etat = -1;
	while (cmd->cmd[i])
	{
		j = 0;
		while (cmd->cmd[i][j])
		{
			etat = searching_etat(cmd->cmd[i][j], etat);
			if (cmd->cmd[i][j] == '$' && etat != 2 && cmd->cmd[i][j - 1] != '\\')
			{
				cmd->cmd[i] = sub_strs(cmd->cmd[i], j, our_envp, sortie);
				j = 0;
				if (cmd->cmd[i] == 0)
					return (0); // error malloc false code return 0
			}
			else
				j++;
		}
		i++;
	}
	return (1); // error malloc bon code return 1
}
