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
