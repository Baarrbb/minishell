#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "parsing.h"

static int	is_spe_char(int c)
{
	if (c == '>' || c == '<' || c == '\'' || c == '\"'
		|| c == '|')
		return (1);
	return (0);
}

static int	file_char(int c)
{
	if (ft_isprint(c) && !is_spe_char(c) && c != ' ')
		return (1);
	return (0);
}

static int	is_redir(char *line)
{
	if (line[0] == '>' && line[1] != '>')
		return (1);
	if (line[0] == '>' && line[1] == '>')
		return (2);
	if (line[0] == '<' && line[1] != '<')
		return (3);
	if (line[0] == '<' && line[1] == '<')
		return (4);
	return (0);
}

static int	check_filename(char *line)
{
	int		i;

	i = 0;
	while (file_char(line[i]))
		i++;
	return (i);
}

static int	check_quote(char *line)
{
	while (*line != '\'' && *line != '\"' && *line)
		line++;
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
	return (1);
}

static int	between_quotes(char *line, int i)
{
	while (line[i] != '>' && line[i] != '<' && line[i] != '|'
		&& line[i] != '\'' && line[i] != '\"' && line[i])
		i++;
	if (line[i] == '\'')
	{
		i++;
		while (line[i] != '\'')
			i++;
		i++;
	}
	else if (line[i] == '\"')
	{
		i++;
		while (line[i] != '\"')
			i++;
		i++;
	}
	return (i);
}

static int	check_cmd_args(char	*line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	if (!check_quote(line))
	{
		printf("bash: syntax error quote expected\n");
		return (-1);
	}
	else
	{
		if (ft_strchr(line, '\'') || ft_strchr(line, '\"'))
			i = between_quotes(line, i);
		else
		{
			while (line[i] != '>' && line[i] != '<' && line[i] != '|'
				&& line[i])
				i++;
			return (i);
		}
	}
	return (i);
}

static char *trim_space(char *line)
{
	while (line && (*line == ' ' || *line == '	'))
		line++;
	return (line);
}

static char	*check_redir(char *line)
{
	int		redir;
	int		filename;

	line = trim_space(line);
	redir = is_redir(line);
	while (redir)
	{
		if (redir == 2 || redir == 4)
			line += 2;
		else if (redir == 1 || redir == 3)
			line++;
		line = trim_space(line);
		filename = check_filename(line);
		if (!filename)
			return (0);
		else
			line += filename;
		line = trim_space(line);
		redir = is_redir(line);
	}
	return (line);
}

static int	first_check_grammar(char *line)
{
	int		cmd;

	line = trim_space(line);
	if (!line || line[0] == 0 || line[0] == '|'
		|| line[0] == '&' || line[0] == ';')
		return (0);
	while (*line)
	{
		line = check_redir(line);
		cmd = check_cmd_args(line);
		if (!cmd || cmd == -1)
			return (0);
		line += cmd;
		line = trim_space(line);
		line = check_redir(line);
		line = trim_space(line);
		if (!line)
			return (0);
		if (line[0] == '|')
		{
			line++;
			line = trim_space(line);
			if (!line || !line[0])
				return (0);
		}
	}
	return (1);
}

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("Minishell $ ");
		rl_on_new_line();
		if (!first_check_grammar(line))
			printf("MAUVAISE SYNTAXE\n");
		else
			printf("Good\n");
		free(line);
	}
	return (0);
}
