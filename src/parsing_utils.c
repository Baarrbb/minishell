/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:27:26 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 00:29:09 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dst_null(char	*dst)
{
	dst = (char *)malloc(1 * sizeof(char));
	if (!dst)
		return (NULL);
	dst[0] = '\0';
	return (dst);
}

char	*strjoin(char *dst, char *s)
{
	char	*res;
	int		i;
	int		j;

	if (!s)
		return (NULL);
	if (!dst)
		dst = dst_null(dst);
	res = (char *)malloc((ft_strlen(s) + ft_strlen(dst) + 1) * sizeof(char));
	if (!res)
	{
		free(dst);
		return (NULL);
	}
	i = -1;
	j = -1;
	while (dst[++i])
		res[i] = dst[i];
	while (s[++j])
		res[i++] = s[j];
	res[i] = '\0';
	free(dst);
	return (res);
}

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

static char **copy_env_null(void)
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
