#include "parsing.h"

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

char	*trim_space(char *line)
{
	if (line)
	{
		while (*line == ' ' || *line == '	')
			line++;
	}
	return (line);
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
	// printf("check_quote\n");
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
	int		i;
	int		j;

	i = 0;
	j = 0;
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
			while (is_spe_char(line[i]))
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

static int	error_syntax(char **args, char *line)
{
	char	*token;

	token = 0;
	if (line[0] == '|')
		token = "|";
	else if (line[0] == 0 && !args[1])
		token = "newline";
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
		else if (args[i][0] == '>' && args[i][1] == '>')
			return (error_syntax(&args[i], &args[i][2]));
		else if (args[i][0] == '<' && args[i][1] == '<')
			return (error_syntax(&args[i], &args[i][2]));
		else if (args[i][0] == '>' && args[i][1] != '>')
			return (error_syntax(&args[i], &args[i][1]));
		else if (args[i][0] == '<' && args[i][1] != '<')
			return (error_syntax(&args[i], &args[i][1]));
	}
	return (1);
}

static int	check_syntax_pipe(char **args, int size)
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

static int	check_syntax(char **args, int size)
{
	return (check_syntax_redir(args) && check_syntax_pipe(args, size));
}

static t_cmd	*fill_cmd(t_cmd **pipe, char **args)
{
	int	i;

	i = -1;
	cmd = ft_calloc(1, sizeof(t_cmd *))
	while (args[++i])
	{
		if (!ft_strncmp(args[i], ">", ft_strlen(args[i])))
		{

		}
		else if (!ft_strncmp(args[i], ">>", ft_strlen(args[i])))
		{
			
		}
		else if (!ft_strncmp(args[i], "<", ft_strlen(args[i])))
		{
			
		}
		else if (!ft_strncmp(args[i], "<<", ft_strlen(args[i])))
		{
			
		}
		else if (!ft_strncmp(args[i], "|", ft_strlen(args[i])))
		{
			
		}
		else
		{

		}
	}
}

static t_cmd	*check_line(char *line, t_cmd *cmd, t_cmd **pipe)
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
	for(int i = 0; args[i]; i++)
		printf("%d : .%s.\n", i, args[i]);
	printf("\n");
	if(!check_syntax(args, size))
		return (0);
	return (fill_cmd(pipe, line));
}



int main(void)
{
	t_cmd *cmd = 0;
	t_cmd *pipe = 0;

	printf("\n1\n");
	check_line("  \"cat line | lala\"   ", &cmd, &pipe); // 1
	printf("\n2\n");
	check_line("cat   \"line | lala\"  ", &cmd, &pipe); // 2
	printf("\n3\n");
	check_line("  cat < test > \"line | lala\"  ", &cmd, &pipe); // 5
	printf("\n4\n");
	check_line("  cat < test > line | wc ", &cmd, &pipe); // 7
	printf("\n5\n");
	check_line("  cat<test>>line|wc>\"lalala\"", &cmd, &pipe); // 10

}