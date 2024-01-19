/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:05:13 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 17:21:40 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_cmd(t_cmd **cmd, char **args)
{
	int	nb_args;
	int	i;

	nb_args = 0;
	i = -1;
	while (args[++i] && args[i][0] != '>' && args[i][0] != '<'
		&& args[i][0] != '|')
		nb_args++;
	(*cmd)->cmd = ft_calloc(nb_args + 1, sizeof(char *));
	if (!(*cmd)->cmd)
		return ;
	i = -1;
	while (args[++i] && args[i][0] != '>' && args[i][0] != '<'
		&& args[i][0] != '|')
		(*cmd)->cmd[i] = ft_strdup(args[i]);
}

static void	add_arg(t_cmd **cmd, char **args)
{
	int		i;
	int		j;
	char	**new_cmd;

	i = 0;
	j = 0;
	while ((*cmd)->cmd[i])
		i++;
	while (args[j] && args[j][0] != '>' && args[j][0] != '<'
		&& args[j][0] != '|')
		j++;
	new_cmd = ft_calloc(i + j + 1, sizeof(char *));
	if (!new_cmd)
		return ;
	i = -1;
	while ((*cmd)->cmd[++i])
		new_cmd[i] = ft_strdup((*cmd)->cmd[i]);
	j = -1;
	while (args[++j] && args[j][0] != '>' && args[j][0] != '<'
		&& args[j][0] != '|')
		new_cmd[i++] = ft_strdup(args[j]);
	free_char_tab((*cmd)->cmd);
	(*cmd)->cmd = new_cmd;
}

static int	fill_cmd_cmd(t_cmd **cmd, t_cmd ***pipe, char **args, int i)
{
	int	j;

	j = -1;
	get_cmd(cmd, &args[i]);
	while ((*cmd)->cmd[++j])
		i++;
	ft_lstadd_back_bis(*pipe, *cmd);
	return (i);
}

void	fill_cmd(t_cmd **pipe, char **args)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return ;
	ft_memset(cmd, 0, sizeof(t_cmd));
	while (args[i] && ft_strncmp(args[i], "|", ft_strlen(args[i])))
	{
		i += is_redir(&cmd, args, i);
		if (!cmd->cmd)
			i = fill_cmd_cmd(&cmd, &pipe, args, i);
		else if (cmd->cmd)
		{
			add_arg(&cmd, &args[i]);
			while (args[i] && args[i][0] != '>' && args[i][0] != '<'
				&& args[i][0] != '|')
				i++;
		}
	}
	if (args[i] && !ft_strncmp(args[i], "|", ft_strlen(args[i])))
		fill_cmd(pipe, &args[i + 1]);
}
