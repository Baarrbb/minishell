/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 16:37:27 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/18 15:44:13 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	good_flag(char *arguments)
{
	int	i;

	i = 1;
	if (!arguments)
		return (1);
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
	next_line = 1;
	if (cmds_args[1] == NULL)
		return ;
	while (good_flag(cmds_args[i]) == 0)
	{
		next_line = 0;
		i++;
	}
	while (cmds_args[i])
	{
		printf("%s", cmds_args[i]);
		if (cmds_args[i + 1] != NULL)
			printf(" ");
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
	free((*arr)[i]);
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
			if (ft_strncmp(copy_env[i], cmds[j], ft_strlen(cmds[j])) == 0)
				if (copy_env[i][ft_strlen(cmds[j])] == '=')
					remove_elem(&copy_env, i);
			j++;
		}
		i++;
	}
	i = 0;
}
void    our_env(char **env)
{
    int    i;

    i = -1;
    if (env)
    {
        while (env[++i])
            printf("%s\n", env[i]);
    }
}

static int    get_size(char **env)
{
    int    i;

    i = 0;
    if (env)
    {
        while (env[i])
            i++;
    }
    return (i);
}

static void    print_export_alpha(char **env)
{
    char    *tmp;
    int        i;
    int        j;
    int        size;

    size = get_size(env);
    i = -1;
    if (!env)
        return ;
    while (++i < size - 1)
    {
        j = -1;
        while (++j < size - i - 1)
        {
            if (ft_strncmp(env[j], env[j + 1], ft_strlen(env[j])) > 0 )
            {
                tmp = env[j];
                env[j] = env[j + 1];
                env[j + 1] = tmp;
            }
        }
    }
    i = -1;
    while (env[++i])
        printf("declare -x %s\n", env[i]);
}

static void    put_var(char ***env, char *var)
{
    char    **tmp;
    char    **new;
    int        i;

    tmp = *env;
    new = ft_calloc(get_size(tmp) + 3, sizeof(char *));
    if (!new)
        return ;
    i = -1;
    while (tmp[++i])
        new[i] = ft_strdup(tmp[i]);
    new[i] = ft_strdup(var);
    free_char_tab(*env);
    *env = new;
}

void    our_export(t_cmd *cmd, char ***env)
{
    int    i;

    i = 0;
    while (cmd->cmd[++i])
    {
        if (ft_strchr(cmd->cmd[i], '='))
            put_var(env, cmd->cmd[i]);
    }
    if (i == 1)
        print_export_alpha(*env);
    
}

void our_exit(t_cmd *everything)
{
	exit(EXIT_SUCCESS);
}

void	builtingo(t_cmd *cmd, char ***env)
{
	int fdinsave;
	int fdoutsave;

	fdinsave = dup(0);
	fdoutsave = dup(1);
	redirections_out(cmd);
	redirections_in(cmd);
	if (!ft_strncmp(cmd->cmd[0], "echo", ft_strlen("echo")))
		our_echo(cmd->cmd);
	else if (!ft_strncmp(cmd->cmd[0], "cd", ft_strlen("cd")))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "pwd", ft_strlen("pwd")))
		our_pwd();
	else if (!ft_strncmp(cmd->cmd[0], "export", ft_strlen("export")))
		//cmd->builtin = 1;	
		our_export(cmd, env);
	else if (!ft_strncmp(cmd->cmd[0], "unset", ft_strlen("unset")))
		our_unset(cmd->cmd, *env);
	else if (!ft_strncmp(cmd->cmd[0], "env", ft_strlen("env")))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "exit", ft_strlen("exit")))
		our_exit(cmd);
	dup2(fdoutsave, 1);
}
