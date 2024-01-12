/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:37:27 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/12 16:18:11 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	good_flag(char *arguments)
{
	int	i;

	i = 1;
	if (!arguments)
		return (0);
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

void	our_echo(char **cmds_args)
{
	int	next_line;
	int	i;

	i = 1;
	next_line = 0;
	if (cmds_args[1] == NULL)
		return ;
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

void	our_pwd()
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

void	our_unset(char **cmds, char **copy_env)
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
			j++;
		}
		i++;
	}
	i = 0;
}

void our_exit()
{
	exit(EXIT_SUCCESS);
}

/*void	exit()
{
	free_char_tab(command);
	free_list(&pipe);
	exit(1);
}*/
void	builtingo(t_cmd *cmd, char **env)
{
	if (!ft_strncmp(cmd->cmd[0], "echo", ft_strlen("echo")))
		our_echo(cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "cd", ft_strlen("cd")))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "pwd", ft_strlen("pwd")))
		our_pwd();
	else if (!ft_strncmp(cmd->cmd[0], "export", ft_strlen("export")))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "unset", ft_strlen("unset")))
		our_unset(cmd->cmd, env);
	else if (!ft_strncmp(cmd->cmd[0], "env", ft_strlen("env")))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "exit", ft_strlen("exit")))
		our_exit();
}