/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:44:01 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 18:38:30 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	error_syntax(char **args, char *line)
{
	char	*token;

	token = 0;
	if ((!line || line[0] == 0) && !args[1])
		token = "newline";
	else if (line[0] == '|')
		token = "|";
	else if ((line[0] == '>' && line[1] == '>')
		|| (args[1] && args[1][0] == '>' && args[1][1] == '>'))
		token = ">>";
	else if ((line[0] == '<' && line[1] == '<')
		|| (args[1] && args[1][0] == '<' && args[1][1] == '<'))
		token = "<<";
	else if (line[0] == '>' || (args[1][0] == '>'))
		token = ">";
	else if (line[0] == '<' || (args[1][0] == '<'))
		token = "<";
	if (token)
	{
		printf("%s`%s\'\n", ERROR_MSG, token);
		return (0);
	}
	return (1);
}

static int	check_syntax_redir(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (args[i][0] == '<' && args[i][1] == '>')
		{
			printf("%s`newline\'\n", ERROR_MSG);
			return (0);
		}
		else if ((args[i][0] == '>' && args[i][1] == '>')
			|| (args[i][0] == '<' && args[i][1] == '<'))
		{
			if (!error_syntax(&args[i], &args[i][2]))
				return (0);
		}
		else if ((args[i][0] == '>' && args[i][1] != '>')
			|| (args[i][0] == '<' && args[i][1] != '<'))
		{
			if (!error_syntax(&args[i], &args[i][1]))
				return (0);
		}
	}
	return (1);
}

static int	check_syntax_pipe(char **args, int size)
{
	if (*args && args[0])
	{
		if (args[0][0] == '|' && args[0][1] == '|')
		{
			printf("%s`||\'\n", ERROR_MSG);
			return (0);
		}
		else if (args[0][0] == '|')
		{
			printf("%s`|\'\n", ERROR_MSG);
			return (0);
		}
		else if (args[size - 1][0] == '|')
		{
			printf("%s`|\'\n", ERROR_MSG);
			return (0);
		}
		return (1);
	}
	return (0);
}

int	check_syntax(char **args, int size)
{
	return (check_syntax_redir(args) && check_syntax_pipe(args, size));
}
