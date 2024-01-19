/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:23:33 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 00:20:38 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_nb_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->cmd && cmd->cmd[0])
	{
		while (cmd->cmd[i])
			i++;
	}
	return (i);
}

//si on trouve pas OLDPWD 
// (parex env -i ./minishell) faut quand meme rajouter la var OLDPWD
static void	refresh_env_pwd(char ***env, char *oldpwd)
{
	char	**tmp;
	int		i;
	char	pwd[PATH_MAX];

	tmp = *env;
	i = -1;
	getcwd(pwd, PATH_MAX);
	while (tmp[++i])
	{
		if (!ft_strncmp(tmp[i], "OLDPWD=", ft_strlen("OLDPWD=")))
		{
			tmp[i] = 0;
			tmp[i] = strjoin(tmp[i], "OLDPWD=");
			tmp[i] = strjoin(tmp[i], oldpwd);
		}
		if (!ft_strncmp(tmp[i], "PWD=", ft_strlen("PWD=")))
		{
			tmp[i] = 0;
			tmp[i] = strjoin(tmp[i], "PWD=");
			tmp[i] = strjoin(tmp[i], pwd);
		}
	}
}

static void	ret_cd(int ret, char **env, char *oldpwd)
{
	if (ret == 0)
		refresh_env_pwd(&env, oldpwd);
	else if (ret < 0)
		perror(ERROR_CD);
	else
		return ;
}

static void	move_cd(char *path, char **env, char *oldpwd)
{
	int		ret;

	ret = 1;
	if (!path || !ft_strncmp(path, "--\0", 3) || !ft_strncmp(path, "~\0", 2))
	{
		if (get_ourenv("HOME", env, 0))
			ret = chdir(get_ourenv("HOME", env, 0));
		else
			printf("%s HOME not set\n", ERROR_CD);
	}
	else if (!ft_strncmp(path, "-\0", 2) && get_ourenv("OLDPWD", env, 0))
	{
		printf("%s\n", get_ourenv("OLDPWD", env, 0));
		ret = chdir(get_ourenv("OLDPWD", env, 0));
	}
	else if (!ft_strncmp(path, "-\0", 2) && !get_ourenv("OLDPWD", env, 0))
		printf("%s OLDPWD not set\n", ERROR_CD);
	else if (path[0] == '-' && path[1] == '-' && path[3])
		printf("%s: --: invalid option\n", ERROR_CD);
	else if (path[0] == '-' && path[1])
		printf("%s: -%c: invalid option\n", ERROR_CD, path[1]);
	else
		ret = chdir(path);
	ret_cd(ret, env, oldpwd);
}

void	our_cd(t_cmd *cmd, char **env)
{
	int		nb_args;
	char	*arg;
	char	oldpwd[PATH_MAX];

	nb_args = get_nb_args(cmd);
	if (nb_args > 2)
	{
		printf("%s too many arguments\n", ERROR_CD);
		return ;
	}
	getcwd(oldpwd, PATH_MAX);
	arg = cmd->cmd[1];
	move_cd(arg, env, oldpwd);
}
