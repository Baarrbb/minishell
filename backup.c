// static int	is_redir(char *line)
// {
// 	if (line[0] == '>' && line[1] != '>')
// 		return (1);
// 	if (line[0] == '>' && line[1] == '>')
// 		return (2);
// 	if (line[0] == '<' && line[1] != '<')
// 		return (3);
// 	if (line[0] == '<' && line[1] == '<')
// 		return (4);
// 	return(0);
// }

// static int check_filename(char *line)
// {
// 	int		i;
// 	i = 0;
// 	while (file_char(line[i]))
// 		i++;
// 	return (i);
// }

// static int	check_cmd_args(char	*line)
// {
// 	int	i;
// 	i = 0;
// 	if (!check_quote(line))
// 	{
// 		printf("bash: syntax error quote expected\n");
// 		return (-1);
// 	}
// 	else
// 	{
// 		if (ft_strchr(line, '\'') || ft_strchr(line, '\"'))
// 		{
// 			while (line[i] != '>' && line[i] != '<' && line[i] != '|' &&
// 				line[i] != '\'' && line[i])
// 		}
// 		else
// 		{
// 			while (line[i] != '>' && line[i] != '<' && line[i] != '|' && line[i])
// 				i++;
// 			return (i);
// 		}
// 	}
// 	return (0);
// }

// static int	first_check_gramamr(char *line)
// {
// 	char	*linetrim;
// 	char	*tmp;
// 	int		redir;
// 	int		filename;
// 	int		cmd;
// 	int		pipe;
// 	linetrim = ft_strtrim(line, " 	");
// 	tmp = linetrim;
// 	if (linetrim[0] == '|' || linetrim[0] == '&' || linetrim[0] == ';')
// 		return (0);
// 	pipe = 1;
// 	redir = is_redir(linetrim);
// 	cmd = check_cmd_args(linetrim);
// 	while (pipe)
// 	{
// 		while (redir)
// 		{
// 			if (redir == 2 || redir == 4)
// 				linetrim += 2;
// 			else if (redir == 1 || redir == 3)
// 				linetrim++;
// 			linetrim = ft_strtrim(linetrim, " 	");
// 			filename = check_filename(linetrim);
// 			if(!filename)
// 			{
// 				free(tmp);
// 				return (0);
// 			}
// 			else
// 				linetrim += filename;
// 			linetrim = ft_strtrim(linetrim, " 	");
// 			redir = is_redir(linetrim);
// 		}
// 		cmd = check_cmd_args(linetrim);
// 		if (cmd == -1)
// 		{
// 			free(tmp);
// 			return (0);
// 		}
// 		if (cmd)
// 		{
// 			linetrim += cmd;
// 			linetrim = ft_strtrim(linetrim, " 	");
// 		}
// 		redir = is_redir(linetrim);
// 		while (redir)
// 		{
// 			if (redir == 2 || redir == 4)
// 				linetrim += 2;
// 			else if (redir == 1 || redir == 3)
// 				linetrim++;
// 			linetrim = ft_strtrim(linetrim, " 	");
// 			filename = check_filename(linetrim);
// 			if(!filename)
// 			{
// 				free(tmp);
// 				return (0);
// 			}
// 			else
// 				linetrim += filename;
// 			linetrim = ft_strtrim(linetrim, " 	");
// 			redir = is_redir(linetrim);
// 		}
// 		if (linetrim[0] != '|')
// 			pipe = 0;
// 	}
// 	free(tmp);
// 	return (1);
// }


static char	**all_args_w_quote(char **tmp, char **quote)
{
	printf("all_args_w_quote\n");
	int		i;
	int		j;
	char	**cmd;

	i = 0;
	j = 1;
	while (tmp[i])
		i++;
	while (quote[j])
		j++;
	cmd = ft_calloc(i + j + 1, sizeof(char *));
	if (!cmd)
		return (0);
	i = -1;
	while (tmp[++i])
		cmd[i] = ft_strdup(tmp[i]);
	j = -1;
	while (quote[++j])
		cmd[i++] = ft_strdup(quote[j]);
	free_char_tab(tmp);
	free_char_tab(quote);
	return (cmd);
}

static int	get_nb_args(char **quote)
{
	printf("get_nb_args\n");
	int	i;
	int	last_pos;
	int	count;

	i = -1;
	count = 0;
	while (quote[++i])
	{
		last_pos = ft_strlen(quote[i]) - 1;
		if ((quote[i][0] == '\"' && quote[i][last_pos] == '\"')
			|| (quote[i][0] == '\'' && quote[i][last_pos] == '\''))
			count++;
		else if (quote[i][0] == '\"')
		{
			while (quote[i][last_pos] != '\"')
			{
				i++;
				last_pos = ft_strlen(quote[i]) - 1;
			}
			count++;
		}
		else if (quote[i][0] == '\'')
		{
			while (quote[i][last_pos] != '\'')
			{
				i++;
				last_pos = ft_strlen(quote[i]) - 1;
			}
			count++;
		}
		else
			count++;
	}
	return (count);
}

static char	**get_args_w_quote(int nb_args, char *line_quote)
{
	printf("get_args_w_quote\n");
	char	**good_quote;
	int		i;
	int		j;
	char	*trim;
	char	*tmp;

	j = 0;
	good_quote = ft_calloc(nb_args + 1, sizeof(char *));
	trim = ft_strtrim(line_quote, " 	");
	tmp = trim;
	while (*trim)
	{
		i = 0;
		if (*trim == '\"')
		{
			i++;
			while (trim[i] != '\"')
				i++;
			good_quote[j] = ft_substr(trim, 0, i + 1);
			trim += i + 1;
			trim = trim_space(trim);
		}
		else if (*trim == '\'')
		{
			i++;
			while (trim[i] != '\'')
				i++;
			good_quote[j] = ft_substr(trim, 0, i + 1);
			trim += i + 1;
			trim = trim_space(trim);
		}
		else
		{
			while (*trim == ' ')
				trim++;
			if (*trim == '>' || *trim == '<')
				break;
			while (trim[i] != ' ')
				i++;
			good_quote[j] = ft_substr(trim, 0, i);
			trim += i + 1;
		}
		j++;
	}
	free(tmp);
	return (good_quote);
}

static char	**args_w_quote(char *line_quote)
{
	printf("args_w_quote\n");
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
	nb_args = get_nb_args(quote);
	good_quote = get_args_w_quote(nb_args, line_quote);
	free_char_tab(quote);
	return (good_quote);
}
