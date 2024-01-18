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

static char	**fill_args_w_quote(char **args, char *line)
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
		args[j] = ft_substr(line, 0, i);
		j++;
		line += i;
		while (is_space(*line) && *line)
			line++;
		i = 0;
		if (is_spe_char(line[i]))
		{
			i++;
			args[j] = ft_substr(line, 0, i);
			j++;
			line += i;
			i = 0;
		}
	}
	return (args);
}

static t_cmd *fill_cmd_cmd(char **args)
{

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
	printf("nb args %d\n", get_nb_args(line));
	fill_args_w_quote(args, line);
	for(int i = 0; args[i]; i++)
		printf("%d : .%s.\n", i, args[i]);
	fill_cmd_cmd(args);
	return (1);
}

int main(void)
{
	check_line("  \"cat line | lala\"   "); // 1
	check_line("cat   \"line | lala\"  "); // 2
	check_line("  cat < test > \"line | lala\"  "); // 5
	check_line("  cat < test > line | wc "); // 7
	check_line("  cat<test>line|wc>\"lalala\""); // 9
}