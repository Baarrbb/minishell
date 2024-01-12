# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <wait.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include "libft.h"

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

void	free_char_tab(char **tab)
{
	int	i;

	i = -1;
	while (tab[++i])
		free(tab[i]);
	free(tab);
}

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

// static int	get_nb_args(char **quote)
// {
// 	int	i;
// 	int	last_pos;
// 	int	count;
// 	i = -1;
// 	count = 0;
// 	while (quote[++i])
// 	{
// 		last_pos = ft_strlen(quote[i]) - 1;
// 		if ((quote[i][0] == '\"' && quote[i][last_pos] == '\"')
// 			|| (quote[i][0] == '\'' && quote[i][last_pos] == '\''))
// 			count++;
// 		else if (quote[i][0] == '\"')
// 		{
// 			while (quote[i][last_pos] != '\"' && quote[i])
// 			{
// 				i++;
// 				if (!quote[i])
// 					return (count += 1);
// 				else
// 					last_pos = ft_strlen(quote[i]) - 1;
// 			}
// 			count++;
// 		}
// 		else if (quote[i][0] == '\'')
// 		{
// 			while (quote[i][last_pos] != '\'' && quote[i])
// 			{
// 				i++;
// 				if (!quote[i])
// 					return (count += 1);
// 				else
// 					last_pos = ft_strlen(quote[i]) - 1;
// 			}
// 			count++;
// 		}
// 		else
// 			count++;
// 	}
// 	printf("count %d\n", count);
// 	return (count);
// }

// static char **get_args_w_quote(char **quote, int nb_args, char *line_quote)
// {
// 	int		i;
// 	int		j;
// 	int		last_pos;
// 	char	**good_quote;
// 	i = -1;
// 	j = 0;
// 	good_quote = ft_calloc(nb_args + 1, sizeof(char *));
// 	if (!good_quote)
// 		return (0);
// 	while (quote[++i])
// 	{
// 		last_pos = ft_strlen(quote[i]) - 1;
// 		if ((quote[i][0] == '\"' && quote[i][last_pos] == '\"')
// 			|| (quote[i][0] == '\'' && quote[i][last_pos] == '\''))
// 			good_quote[j] = ft_strdup(quote[i]);
// 		else if (quote[i][0] == '\"')
// 		{
// 			good_quote[j] = ft_strdup(quote[i]);
// 			while (quote[i][last_pos] != '\"' && quote[i] != 0)
// 			{
// 				i++;
// 				last_pos = ft_strlen(quote[i]) - 1;
// 				if (!quote[i])
// 					return (good_quote);
// 				good_quote[j] = strjoin(good_quote[j], quote[i]);
// 			}
// 		}
// 		else if (quote[i][0] == '\'')
// 		{
// 			good_quote[j] = ft_strdup(quote[i]);
// 			while (quote[i][last_pos] != '\'' && quote[i] != 0)
// 			{
// 				i++;
// 				last_pos = ft_strlen(quote[i]) - 1;
// 				if (!quote[i])
// 					return (good_quote);
// 				good_quote[j] = strjoin(good_quote[j], quote[i]);
// 			}
// 		}
// 		else
// 			good_quote[j] = ft_strdup(quote[i]);
// 		j++;
// 	}
// 	return (good_quote);
// }

// static char	**args_w_quote(char *line_quote)
// {
// 	char	**quote;
// 	int		nb_args;
// 	char	**good_quote;
// 	int		i;
// 	quote = ft_split(line_quote, ' ');
// 	i = -1;
// 	while (quote[i])
// 		i++;
// 	if (i == 1)
// 		return (quote);
// 	nb_args = get_nb_args(quote);
// 	good_quote = get_args_w_quote(quote, nb_args, line_quote);
// 	return (good_quote);
// }

// static char **args_w_quote(char *line)
// {
// 	int	i = 0;
// 	while (line[i] != '\'' && line[i] != '\"')
// 		i++;
// 	char *line_wo_quote = ft_substr(line, 0, i);
// 	printf("line wo quote : .%s.\n", line_wo_quote);
// 	char *line_w_quote = ft_substr(line, i, ft_strlen(line));
// 	printf("line w quote : .%s.\n", line_w_quote);
// 	char **args_wo_quote = ft_split(line_wo_quote, ' ');
// 	for(int i = 0; args_wo_quote[i]; i++)
// 		printf("args wo : .%s.\n", args_wo_quote[i]);
// 	// char **args_w_quote = 
// 	// for(int i = 0; args_w_quote[i]; i++)
// 	// 	printf("args w : .%s.\n", args_w_quote[i]);
// }


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
		while(!is_space(line[i]) && line[i])
		{
			if (line[i] == '>' || line[i] == '<')
				break;
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
		if (line[i] == '>' || line[i] == '<')
				break;
		count++;
		while (is_space(line[i]) && line[i])
			i++;
	}
	return (count);
}

//  "sed -n   p'sdsdf'd  kk'dsf\"sdf df' 'dsf  df'sdd  \"s/,   $/./p\"   "

static char **fill_args_w_quote(char **args, char *line)
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
		while(!is_space(line[i]) && line[i])
		{
			if (line[i] == '>' || line[i] == '<')
				break;
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
		if (line[i] == '>' || line[i] == '<')
				break;
		args[j] = ft_substr(line, 0, i);
		j++;
		line += i;
		while (is_space(*line) && *line)
			line++;
		i = 0;
	}
	return (args);
}

static char **args_w_quote(char *line)
{
	int		size;
	char	**args;

	size = get_nb_args(line);
	args = ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (0);
	args = fill_args_w_quote(args, line);
	return (args);
}


int main()
{
	char *test = "sed -n   p'sdsdf'd  kk'dsf\"sdf df' 'dsf  df'sdd  \"s/,   $/./p\" > out  ";
	printf("nb args %d\n", get_nb_args(test));
	char **quote = args_w_quote(test);
	printf("BIEN GERE ?? \n\n");
	for(int i = 0; quote[i]; i++)
		printf(".%s.\n", quote[i]);
}