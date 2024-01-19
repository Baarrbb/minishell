/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:36:10 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 17:15:30 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Partie je fais un double tableau en separant les mots des tokens
 * et verification syntaxe
*/


static int	check_quote(char *line)
{
	while (*line != '\'' && *line != '\"' && *line)
		line++;
	while (*line)
	{
		if (*line == '\'')
		{
			line++;
			while (*line != '\'' && *line)
				line++;
			if (*line == 0)
				return (0);
		}
		else if (*line == '\"')
		{
			line++;
			while (*line != '\"' && *line)
				line++;
			if (*line == 0)
				return (0);
		}
		line++;
		while (*line != '\'' && *line != '\"' && *line)
			line++;
	}
	return (1);
}

static int	get_next_quote(char *line, int i)
{
	if (line[i] == '\'' && line[i])
	{
		i++;
		while (line[i] != '\'')
			i++;
	}
	if (line[i] == '\"' && line[i])
	{
		i++;
		while (line[i] != '\"' && line[i])
			i++;
	}
	i++;
	return (i);
}

static int	get_nb_args(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		while (is_space(line[i]) && line[i])
			i++;
		while (!is_space(line[i]) && !is_spe_char(line[i]) && line[i])
			i = get_next_quote(line, i);
		if (i - 1 >= 0 && !is_space(line[i - 1]) && !is_spe_char(line[i - 1]))
			count++;
		if (is_spe_char(line[i]))
		{
			while (is_spe_char(line[i]) && line[i])
				i++;
			count++;
		}
		while (is_space(line[i]) && line[i])
			i++;
	}
	return (count);
}

static char	**fill_args(char **args, char *line)
{
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	while (*line)
	{
		while (is_space(*line) && *line)
			line++;
		i = 0;
		while (!is_space(line[i]) && !is_spe_char(line[i]) && line[i])
			i = get_next_quote(line, i);
		if (i)
			args[j++] = ft_substr(line, 0, i);
		line += i;
		while (is_space(*line) && *line)
			line++;
		i = 0;
		while (is_spe_char(line[i]))
			i++;
		if (i)
			args[j++] = ft_substr(line, 0, i);
		line += i;
	}
	return (args);
}





t_cmd	*check_line(char *line, t_cmd **pipe, char **envp)
{
	char	**args;
	int		size;

	if (!check_quote(line))
	{
		printf("minishell: syntax error quote expected\n");
		return (0);
	}
	size = get_nb_args(line);
	args = ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (0);
	fill_args(args, line);
	if(!check_syntax(args, size))
		return (0);
	return(fill_struct(pipe, args, envp));
}
