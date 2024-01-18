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

static int	error_syntax(char **args)
{
	char	*token;
	int		i;
	int		count_sup;
	int		count_inf;

	token = 0;
	i = -1;
	count_sup = 0;
	count_inf = 0;
	if (!ft_strncmp(args[0], ">", 2))
	{
		while (args[++i] && !ft_strncmp(args[i], ">", 2))
			count_sup++;
	}
	if (!ft_strncmp(args[0], "<\0", 2))
	{
		while (args[++i] && !ft_strncmp(args[i], "<\0", 2))
			count_inf++;
	}
	if (count_sup >= 2)
		printf("%s `>>'\n", ERROR_MSG);
	else if (count_sup == 1)
		printf("%s `>'\n", ERROR_MSG);
	if (count_inf >= 2)
		printf("%s `<<'\n", ERROR_MSG);
	else if (count_inf == 1)
		printf("%s `<'\n", ERROR_MSG);
	return (0);
}

static int	check_syntax_redir(char **args, char *line)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (args[++i])
	{
		while (!ft_strncmp(args[i], ">\0", 2) && args[i])
		{
			if (!args[i + 1])
			{
				printf("%s `newline'\n", ERROR_MSG);
				return (0);
			}
			count++;
			if (count > 2)
				return (error_syntax(&args[i]));
			i++;
			if (args[i + 1] && count > 2 && !ft_strncmp(args[i + 1], "<\0", 2))
				return (error_syntax(&args[i + 1]));
		}
		count = 0;
		while (!ft_strncmp(args[i], "<\0", 2) && args[i])
		{
			if (!args[i + 1])
			{
				printf("%s `newline'\n", ERROR_MSG);
				return (0);
			}
			count++;
			if (count > 2)
				return (error_syntax(&args[i]));
			i++;
			if (args[i + 1] && count > 2 && !ft_strncmp(args[i + 1], ">\0", 2))
				return (error_syntax(&args[i + 1]));
		}
		count = 0;
	}
	return (1);
}

static int	check_syntax(char **args, char *line)
{
	// check_syntax_pipe(args, size);
	return (check_syntax_redir(args, line));
}

static int	check_line(char *line)
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
	// printf("nb args %d\n", size);
	fill_args(args, line);
	// for(int i = 0; args[i]; i++)
	// 	printf("%d : .%s.\n", i, args[i]);
	// printf("\n");
	if(!check_syntax(args, line))
		return (0);
	return (1);
}

int main(void)
{
	// printf("1\n");
	// check_line("  \"cat line | lala\"   "); // 1
	// printf("2\n");
	// check_line("cat   \"line | lala\"  "); // 2
	// printf("3\n");
	// check_line("  cat < test > \"line | lala\"  "); // 5
	// printf("4\n");
	// check_line("  cat < test > line | wc "); // 7
	// printf("5\n");
	// check_line("  cat<test>>line|wc>\"lalala\""); // 10

	printf("6\n");
	check_line(">"); 
	printf("7\n");
	check_line("<"); 
	printf("8\n");
	check_line("  >> "); 
	printf("9\n");
	check_line("  << ");
	printf("10\n");
	check_line("  >> ");
	printf("11 a gerer differemment\n");
	check_line("  <> "); //a voir
	printf("12\n");
	check_line(">>>>>");
	printf("13\n");
	check_line(">>>>>>>>>>>>");
	printf("14\n");
	check_line("<<<<<");
	printf("15\n");
	check_line("<<<<<<<<<<");
	printf("16\n");
	check_line("> > > >");
	printf("17\n");
	check_line(">> >> >> >>");
	printf("18\n");
	check_line(">>>> >> >> >>");
}