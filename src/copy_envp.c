/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 17:08:59 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 17:09:14 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_shlvl(char **envp)
{
	int		i;
	int		j;
	char	*new;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "SHLVL", ft_strlen("SHLVL")))
		{
			j = 0;
			while (envp[i][j] != '=')
				j++;
			j += 1;
			if (ft_atoi(envp[i] + j))
			{
				new = ft_itoa(ft_atoi(envp[i] + j) + 1);
				free(envp[i]);
				envp[i] = 0;
				envp[i] = strjoin(ft_strdup("SHLVL="), new);
				free(new);
			}
		}
	}
}

static char	**copy_env_null(void)
{
	char	**new_env;
	char	pwd[PATH_MAX];

	getcwd(pwd, PATH_MAX);
	new_env = ft_calloc(4, sizeof(char *));
	if (!new_env)
		return (0);
	new_env[0] = strjoin(new_env[0], "PWD=");
	new_env[0] = strjoin(new_env[0], pwd);
	new_env[1] = strjoin(new_env[1], "SHLVL=");
	new_env[1] = strjoin(new_env[1], "1");
	new_env[2] = strjoin(new_env[2], "_=");
	new_env[2] = strjoin(new_env[2], "./minishell");
	return (new_env);
}

char	**copy_env(char **envp)
{
	int		i;
	char	**cpy_env;

	i = 0;
	if (!envp[0])
	{
		cpy_env = copy_env_null();
		add_shlvl(cpy_env);
		return (cpy_env);
	}
	while (envp[i])
		i++;
	cpy_env = ft_calloc(i + 1, sizeof(char *));
	if (!cpy_env)
		return (0);
	i = -1;
	while (envp[++i])
		cpy_env[i] = ft_strdup(envp[i]);
	add_shlvl(cpy_env);
	return (cpy_env);
}
