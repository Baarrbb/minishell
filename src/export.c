/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 20:32:20 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/17 16:21:53 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	get_size(char **env)
{
	int		i;

	i = 0;
	if (env)
	{
		while (env[i])
			i++;
	}
	return (i);
}

static char	**cpy_env_sort(char **env)
{
	char	**sort;
	int		size;
	int		i;

	if (!env)
		return (0);
	size = get_size(env);
	i = -1;
	sort = ft_calloc(size + 1, sizeof(char *));
	if (!sort)
		return (0);
	while (env[++i])
		sort[i] = ft_strdup(env[i]);
	return (sort);
}

static void	print_export_alpha(char **env)
{
	char	*tmp;
	int		i;
	int		j;
	char	**sort;

	sort = cpy_env_sort(env);
	i = -1;
	while (++i < get_size(env) - 1)
	{
		j = -1;
		while (++j < get_size(env) - i - 1)
		{
			if (ft_strncmp(sort[j], sort[j + 1], ft_strlen(sort[j])) > 0)
			{
				tmp = sort[j];
				sort[j] = sort[j + 1];
				sort[j + 1] = tmp;
			}
		}
	}
	i = -1;
	while (sort[++i])
		printf("declare -x %s\n", sort[i]);
	free_char_tab(sort);
}

static void	put_var(char ***env, char *var)
{
	char	**tmp;
	char	**new;
	int		i;

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

void	our_export(t_cmd *cmd, char ***env)
{
	int	i;

	i = 0;
	while (cmd->cmd[++i])
	{
		if (ft_strchr(cmd->cmd[i], '='))
			put_var(env, cmd->cmd[i]);
	}
	if (i == 1 && *env)
		print_export_alpha(*env);
		
}
