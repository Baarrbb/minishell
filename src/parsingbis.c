/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingbis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:36:10 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/18 17:15:37 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

static int	is_spe_char(int c)
{
	// printf("is_spe_char\n");
	// ajouter  c == '$' ????
	if (c == '>' || c == '<' || c == '|') //|| c == '\'' || c == '\"'
		return (1);
	return (0);
}

static int	check_quote(char *line)
{
	printf("check_quote\n");
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
		}
		if (!is_space(line[i - 1]) && !is_spe_char(line[i - 1]))
			count++;
		if (is_spe_char(line[i]))
		{
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
	char	*arg;
	int		i;
	int		j;

	i = 0;
	j = 0;
	arg = 0;
	while (*line && line[i])
	{
		while (is_space(*line) && *line)
			line++;
		while (!is_space(line[i]) && !is_spe_char(line[i]) && line[i])
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
		}
		if (i != 0)
		{
			args[j] = ft_substr(line, 0, i);
			j++;
			line += i;
			while (is_space(*line) && *line)
				line++;
			i = 0;
		}
		if (is_spe_char(line[i]))
		{
			i++;
			args[j] = ft_substr(line, 0, i);
			j++;
			line += i;
			i = 0;
			continue ;
		}
	}
	return (args);
}

static int	check_syntax(char **args, int size)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (++i < size - 1)
	{
		if (!ft_strncmp(args[i], ">\0", 2))
		{
			while ()
		}
	}
}


static int	check_line(t_cmd **cmd, char *line)
{
	if (!check_quote(line))
	{
		printf("minishell: syntax error quote expected\n");
		return (0);
	}
	char **args = ft_calloc(get_nb_args(line) + 1, sizeof(char *));
	if (!args)
		return (0);
	int	size = get_nb_args(line);
	printf("nb args %d\n", size);
	fill_args(args, line);
	for(int i = 0; args[i]; i++)
		printf("%d : .%s.\n", i, args[i]);
	if(!check_syntax(args, size))
		return (0);
	return (1);
}


int	main(int ac, char **av, char **envp)
{
	char	*line;
	char	**command;
	t_cmd	*cmd;
	t_cmd	*pipe;
	t_redir	*redir;
	int		i;
	int		yesh_pipe;
	char	**cpy_env;

	(void)ac;
	(void)av;
	i = -1;
	pipe = 0;
	cmd = 0;
	redir = 0;
	cpy_env = copy_env(envp);
	while (1)
	{
		line = readline("Minishell $ ");
		rl_on_new_line();
		// if (!line)
			//exit
		if (line[0] != ' ' && line[0] != 0)
			add_history(line);
		check_line(&cmd, line);
		
		command = 0;
		cmd = 0;
		i = -1;
	}
	return (0);
}