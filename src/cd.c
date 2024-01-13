/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 16:40:53 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/13 21:51:33 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"


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

static void	move_cd(char *path, char **env)
{
	char	oldpwd[PATH_MAX];

	getcwd(oldpwd, PATH_MAX);
	if (chdir(path) != 0)
	{
		if (errno == EACCES)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));//ok mauvaise permission
		if (errno == EFAULT)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == EIO)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == ELOOP)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == ENAMETOOLONG)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno)); // ok
		if (errno == ENOENT)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno)); //ok repertoire n'existe pas "jexistepas"
		if (errno == ENOMEM)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == ENOTDIR)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));//ok pas un repertoire "/bin/bash"
		if (errno == EBADF)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
	}
	else
		refresh_env_pwd(&env, oldpwd);
}

static	char	*recup_home_wo_home(void)
{
	
}

void	our_cd(t_cmd *cmd, char **env)
{
	int		nb_args;
	char	*home;
	char	*arg;

	nb_args = get_nb_args(cmd);
	home = 0;
	if (nb_args > 2)
	{
		printf("%s too many arguments\n", ERROR_CD);
		return ;
	}
	arg = cmd->cmd[1];
	home = get_ourenv("HOME", env);
	printf("HOME %s\n", home);
	if (nb_args == 1 && !home)
		printf("%s HOME not set\n", ERROR_CD);
	else if ((nb_args == 1 || !ft_strncmp(arg, "~", ft_strlen(arg))) && home)
		move_cd(home, env);
	else if (home)
		move_cd(arg, env);
	else
		recup_home_wo_home(); // recuperer home
}

