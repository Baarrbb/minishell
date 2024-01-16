/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 16:54:21 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/16 17:51:45 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	isntaletter(char t)
{
	if ((t >= 65 && t <= 90 ) || (t >= 97 && t <= 122)
		|| (t >= 48 && t <= 57) || t == '_')
		return (1);
	return (0);
}

static char	*replace_by_env(char *command, char *result, int ex_len, int len)
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

static char	*replace_dollar(char *str, int pos, char **our_envp, char *sortie)
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
	printf("result when $ : %s, %s\n", result, variable);
	if (result != NULL)
		str = replace_by_env(str, result, (i - pos), ft_strlen(result));
	else
		str = remove_dollar(str);
	free(variable);
	return (str); // error malloc bon code return 1 || // error malloc false code return 0
}

int	handle_var(t_cmd *cmd, char **our_envp, char *sortie)
{
	int i;
	int	etat;
	int	j;

	i = -1;
	j = 0;
	etat = -1;
	while (cmd->cmd[++i])
	{
		j = 0;
		while (cmd->cmd[i][j])
		{
			etat = in_quotes(cmd->cmd[i][j], etat);
			if (cmd->cmd[i][j] == '$' && etat != 2 && cmd->cmd[i][j - 1] != '\\')
			{
				cmd->cmd[i] = replace_dollar(cmd->cmd[i], j, our_envp, sortie);
				j = 0;
				if (cmd->cmd[i] == 0)
					return (0); // error malloc false code return 0
			}
			else
				j++;
		}
	}
	return (1); // error malloc bon code return 1
}
