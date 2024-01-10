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

static int	get_nb_args(char **quote)
{
	int	i;
	int	last_pos;
	int	count;

	i = -1;
	count = 0;
	while (quote[++i])
	{
		printf("quote[i] .%s.\n", quote[i]);
		last_pos = ft_strlen(quote[i]) - 1;
		if ((quote[i][0] == '\"' && quote[i][last_pos] == '\"')
			|| (quote[i][0] == '\'' && quote[i][last_pos] == '\''))
			count++;
		else if (quote[i][0] == '\"')
		{
			while (quote[i][last_pos] != '\"' && quote[i] != 0)
			{
				printf("qui] .%s.\n", quote[i]);
				i++;
				printf("qui++] .%s.\n", quote[i]);
				if (!quote[i])
				{
					printf("av ret %d\n", count);
					return (count += 1);
				}
				if (quote[i] != 0)
				{
					printf("je ne rentre pas\n");
					last_pos = ft_strlen(quote[i]) - 1;
				}
			}
			printf("seg %d??\n", i);
			count++;
			printf("count %d\n", count);
		}
		else if (quote[i][0] == '\'')
		{
			while (quote[i][last_pos] != '\'' && quote[i])
			{
				i++;
				last_pos = ft_strlen(quote[i]) - 1;
			}
			count++;
		}
		else
			count++;
	}
	printf("count %d\n", count);
	return (count);
}

static char **get_args_w_quote(char **quote, int nb_args, char *line_quote)
{
	int		i;
	int		j;
	int		last_pos;
	char	**good_quote;

	i = -1;
	j = 0;
	good_quote = ft_calloc(nb_args + 1, sizeof(char *));
	if (!good_quote)
		return (0);
	while (quote[++i])
	{
		last_pos = ft_strlen(quote[i]) - 1;
		if ((quote[i][0] == '\"' && quote[i][last_pos] == '\"')
			|| (quote[i][0] == '\'' && quote[i][last_pos] == '\''))
			good_quote[j] = ft_strdup(quote[i]);
		else if (quote[i][0] == '\"')
		{
			good_quote[j] = ft_strdup(quote[i]);
			while (quote[i][last_pos] != '\"')
			{
				i++;
				last_pos = ft_strlen(quote[i]) - 1;
				good_quote[j] = strjoin(good_quote[j], quote[i]);
			}
		}
		else if (quote[i][0] == '\'')
		{
			while (quote[i][last_pos] != '\'')
			{
				i++;
				last_pos = ft_strlen(quote[i]) - 1;
				good_quote[j] = strjoin(good_quote[j], quote[i]);
			}
		}
		else
			printf("pas encore gere\n");
		j++;
	}
	return (good_quote);
}

// static char	**get_args_w_quote(char **quote, int nb_args, char *line_quote)
// {
// 	char	**good_quote;
// 	int		i;
// 	int		j;
// 	char	*trim;
// 	char	*tmp;
// 	j = 0;
// 	good_quote = ft_calloc(nb_args + 1, sizeof(char *));
// 	trim = ft_strtrim(line_quote, " 	");
// 	tmp = trim;
// 	while (*trim)
// 	{
// 		i = 0;
// 		if (*trim == '\"')
// 		{
// 			i++;
// 			while (trim[i] != '\"')
// 				i++;
// 			good_quote[j] = ft_substr(trim, 0, i + 1);
// 			trim += i + 1;
// 			trim = ft_strtrim(trim, " 	");
// 		}
// 		else if (*trim == '\'')
// 		{
// 			i++;
// 			while (trim[i] != '\'')
// 				i++;
// 			good_quote[j] = ft_substr(trim, 0, i + 1);
// 			trim += i + 1;
// 			trim = ft_strtrim(trim, " 	");
// 		}
// 		else
// 		{
// 			while (*trim == ' ')
// 				trim++;
// 			while (trim[i] != ' ')
// 				i++;
// 			good_quote[j] = ft_substr(trim, 0, i);
// 			trim += i + 1;
// 		}
// 		j++;
// 	}
// 	free(tmp);
// 	return (good_quote);
// }

static char	**args_w_quote(char *line_quote)
{
	char	**quote;
	int		nb_args;
	char	**good_quote;
	int		i;

	quote = ft_split(line_quote, ' ');
	i = -1;
	while (quote[i])
		i++;
	if (i == 1)
		return (quote);
	for(int i = 0; quote[i]; i++)
		printf("quote .%s.\n", quote[i]);
	nb_args = get_nb_args(quote);
	printf("klsd %d\n", nb_args);
	good_quote = get_args_w_quote(quote, nb_args, line_quote);
	return (good_quote);
}

int main()
{
	char **quote = args_w_quote("\"jkdshfkjdshfk\",\"jsdhfk jdh\" \"rrrrrrr\"dfsdsf");
	for(int i = 0; quote[i]; i++)
		printf(".%s.\n", quote[i]);
}