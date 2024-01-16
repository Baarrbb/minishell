/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 19:12:54 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/16 17:13:55 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	get_arg(t_cmd *cmd, char *arg, char c, int i)
{
	char	*new;
	char	*sub;
	int		j;

	new = 0;
	sub = 0;
	j = 0;
	while (arg[j] != '\'' && arg[j] != '\"')
		j++;
	while (*arg)
	{
		while (arg[j] != c && arg[j])
			j++;
		sub = ft_substr(arg, 0, j);
		new = strjoin(new, sub);
		free(sub);
		j++;
		arg += j;
		j = 0;
	}
	free(cmd->cmd[i]);
	cmd->cmd[i] = new;
}

static void	quote_cmd_filling(t_cmd *quoting)
{
	int	i;

	i = 0;
	printf("filling\n");
	while (quoting->cmd[i])
	{
		if (quoting->cmd[i][0] == '\'')
			quoting->quote_cmd[i] = 1;
		else if (quoting->cmd[i][0] == '\"')
			quoting->quote_cmd[i] = 2;
		else
			quoting->quote_cmd[i] = 0;
		i++;
	}
}

static void	remove_quotes(t_cmd *cmd)
{
	int		i;
	int		j;
	char	*new;
	char	*tmp;

	i = 0;
	j = 0;
	new = 0;
	while (cmd->cmd[i])
	{
		tmp = cmd->cmd[i];
		if (ft_strchr(tmp, '\'') || ft_strchr(tmp, '\"'))
		{
			while (tmp[j] != '\'' && tmp[j] != '\"')
				j++;
			if (tmp[j] == '\"')
				get_arg(cmd, tmp, '\"', i);
			else
				get_arg(cmd, tmp, '\'', i);
		}
		i++;
	}
}

int	handle_quoting(t_cmd *quoting, char **env, int sortie)
{
	int		i;
	char	*str_sortie;

	while (quoting)
	{
		i = 0;
		if (quoting->cmd != NULL)
		{
			while (quoting->cmd[i])
				i++;
			quoting->quote_cmd = malloc(i * sizeof(int));
			if (quoting->quote_cmd == NULL)
				return (0);
			str_sortie = ft_itoa(sortie);
			quote_cmd_filling(quoting);
			if (handle_var(quoting, env, str_sortie) == 0)
				return (0); // error malloc false code return 
			free(str_sortie);
			remove_quotes(quoting);
		}
		quoting = quoting->next;
	}
	return (1); // error malloc bon code return 1
}
