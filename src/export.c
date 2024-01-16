/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:32:20 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/16 19:19:43 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	get_size(char **env)
{
	int	i;

	i = 0;
	if (env)
	{
		while (env[i])
			i++;
	}
	return (i);
}

static void	print_export_alpha(char **env)
{
	char	*tmp;
	int		i;
	int		j;
	int		size;

	size = get_size(env);
	i = -1;
	while (++i < size - 1)
	{
		j = -1;
		while (++j < size - i - 1)
		{
			if (ft_strncmp(env[j], env[j + 1], ft_strlen(env[j])) > 0)
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

static void	put_var(char ***env, char *var)
{
	char	**tmp;
	char	**new;
	int		i;

	tmp = *env;
	new = ft_calloc(get_size(tmp) + 2, sizeof(char *));
	if (!new)
		return ;
	i = -1;
	while (tmp[++i])
		new[i] = ft_strdup(tmp[i]);
	new[i] = ft_strdup(var);
	free_char_tab(tmp);
	*env = new;
}

void	our_export(t_cmd *cmd, char **env)
{
	int	i;

	i = 0;
	while (cmd->cmd[++i])
	{
		if (ft_strchr(cmd->cmd[i], '='))
			put_var(&env, cmd->cmd[i]);
	}
	if (i == 1)
		print_export_alpha(env);
	our_env(env);
}
