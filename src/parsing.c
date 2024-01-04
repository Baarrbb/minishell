/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:26:22 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/04 20:22:40 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

// cat < test  | grep something > outfile > out2 | wc -l < out2

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

static char	**get_path(char **envp)
{
	char	**path;
	char	**del_path;
	int		i;

	i = -1;
	path = 0;
	del_path = 0;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH", 4))
		{
			del_path = ft_split(envp[i], '=');
			path = ft_split(del_path[1], ':');
			break ;
		}
	}
	free_char_tab(del_path);
	return (path);
}

static char	*check_exist_cmd(char *cmd1, t_cmd *cmd)
{
	int		i;
	char	*full_cmd;
	char	**wo_param;

	full_cmd = 0;
	i = -1;
	wo_param = ft_split (cmd1, ' ');
	while (cmd->path[++i])
	{
		full_cmd = strjoin(full_cmd, cmd->path[i]);
		full_cmd = strjoin(full_cmd, "/");
		full_cmd = strjoin(full_cmd, wo_param[0]);
		if (access(full_cmd, X_OK) == 0)
		{
			free_char_tab(wo_param);
			return (full_cmd);
		}
		free(full_cmd);
		full_cmd = 0;
	}
	free_char_tab(wo_param);
	return (0);
}

static char	**get_cmd(char *line)
{
	int		i;
	char	*cmd;
	char	*cmd_trim;
	char	**cmd_args;

	i = 0;
	while (line[i] != '<' && line[i] != '>')
		i++;
	cmd = (char *)ft_calloc(i + 1, sizeof(char));
	if (!cmd)
		return (NULL);
	i = 0;
	while (line[i] != '<' && line[i] != '>')
	{
		cmd[i] = line[i];
		i++;
	}
	cmd_trim = ft_strtrim(cmd, " ");
	free(cmd);
	cmd_args = ft_split(cmd_trim, ' ');
	free(cmd_trim);
	return (cmd_args);
}

//tous les caracteres speciaux sauf space tab et /
static int	is_spe_char(int c)
{
	// ajouter  c == '$' ????
	if (c == '>' || c == '<' || c == '\'' || c == '\"'
		|| c == '|')
		return (1);
	return (0);
}

static int	file_char(int c)
{
	if (ft_isprint(c) && !is_spe_char(c))
		return (1);
	return (0);
}

static void	error_syntax(char *line)
{
	char *token;

	token = 0;
	if (line[0] == 0)
		token = "newline";
	else if (line[0] == '>' && line[1] == '>')
		token = ">>";
	else if (line[0] == '<' && line[1] == '<')
		token = "<<";
	else if (line[0] == '>')
		token = ">";
	else if (line[0] == '<')
		token = "<";
	printf("%s`%s\'\n", ERROR_MSG, token);
}

static char	*get_filename(t_redir *redir, char *line)
{
	int		i;
	int		count;
	char	*filename;
	char	*filetrim;

	i = -1;
	count = 0;
	while (file_char(line[++i]))
		count++;
	if (count == 0)
	{
		error_syntax(line);
		return (0);
	}
	filename = ft_calloc((count + 1), sizeof(char));
	if (!filename)
		return (0);
	i = 0;
	while (file_char((int)*line))
	{
		filename[i] = (int)*line;
		line++;
		i++;
	}
	filetrim = ft_strtrim(filename, " ");
	free(filename);
	redir->filename = filetrim;
	if (*line == 0)
		return (0);
	return (line);
}

static char	*get_redir(t_redir *cmd, char *line)
{
	while (*line && *line != '<' && *line != '>')
		line++;
	if (*line == '<' && *(line + 1) == '>')
	{
		printf("%s`newline\'\n", ERROR_MSG);
		return (0);
	}
	if (*line == '>' && *(line + 1) != '>')
		cmd->out = 1;
	else if (*line == '>' && *(line + 1) == '>')
		cmd->out_end = 1;
	else if (*line == '<' && *(line + 1) != '<')
		cmd->in = 1;
	else if (*line == '<' && *(line + 1) == '<')
		cmd->in_read = 1;
	return (line);
}

static	int	init_redir(t_cmd *cmd, t_redir *redir, char *line)
{
	t_redir	*new;
	char	*linetrim;
	char	*tmp;
	int		i;

	linetrim = ft_strdup(line);
	tmp = linetrim;
	i = -1;
	while (*linetrim)
	{
		new = ft_calloc(1, sizeof(t_redir));
		if (!new)
			return (0);
		ft_memset(new, 0, sizeof(t_redir));
		linetrim = get_redir(new, linetrim);
		if (!linetrim)
		{
			free(new);
			free(tmp);
			return (0);
		}
		if (new->out || new->in)
			linetrim++;
		else if (new->out_end || new->in_read)
			linetrim += 2;
		while (linetrim && linetrim[++i] == ' ')
			linetrim++;
		linetrim = get_filename(new, linetrim);
		if (!new->filename)
		{
			free(new);
			free(tmp);
			return (0);
		}
		new->next = 0;
		ft_lstadd_back(&redir, new);
		if (ft_strlen(linetrim) == 0)
			break ;
	}
	cmd->redir = redir;
	free(tmp);
	return (1);
}

static t_cmd	*init_cmd(char *line, char **envp, t_redir *redir)
{
	t_cmd	*cmd;
	int		len;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (0);
	ft_memset(cmd, 0, sizeof(t_cmd));
	if (envp != 0)
	{
		cmd->path = get_path(envp);
		if (ft_strchr(line, '<') || ft_strchr(line, '>'))
			cmd->cmd = get_cmd(line);
		else
			cmd->cmd = ft_split(line, ' ');
		cmd->path_cmd = check_exist_cmd(cmd->cmd[0], cmd);
	}
	len = ft_strlen(line);
	if (ft_strnstr(line, ">", len) || ft_strnstr(line, "<", len))
		if (!init_redir(cmd, redir, line))
		{
			free_struct(cmd);
			return (0);
		}
	return (cmd);
}

static t_cmd *check_grammar_line(t_redir *redir, t_cmd *cmd, char *line, char **envp)
{
	char	*linetrim;

	linetrim = ft_strtrim(line, " 	");
	if (ft_strlen(linetrim) == 0)
	{
		free(linetrim);
		return (0);
	}
	if (linetrim[0] == '>' || linetrim[0] == '<')
		cmd = init_cmd(linetrim, 0, redir);
	else
		cmd = init_cmd(linetrim, envp, redir);
	free(linetrim);
	return (cmd);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	char	**command;
	t_cmd	*cmd;
	t_cmd	*pipe;
	t_redir	*redir;
	int		i;

	(void)ac;
	(void)av;
	i = -1;
	pipe = 0;
	cmd = 0;
	redir = 0;
	while (1)
	{
		line = readline("Minishell $ ");
		rl_on_new_line();
		if (line[0] != ' ' && line[0] != 0)
			add_history(line);
		command = ft_split(line, '|');
		while (command[++i])
		{
			cmd = check_grammar_line(redir, cmd, command[i], envp);
			if (cmd)
				ft_lstadd_back_bis(&pipe, cmd);
			else
			{
				free_list(&pipe);
				break;
			}
		}
		print_linked_list(pipe);
		free(line);
		free_char_tab(command);
		free_list(&pipe);
		command = 0;
		cmd = 0;
		i = -1;
	}
	return (0);
}


void	print_redir(t_redir *redir)
{
	for (int i = 0; redir; i++)
	{
		printf("out %d end %d\n", redir->out, redir->out_end);
		printf("in %d read %d\n", redir->in, redir->in_read);
		printf("Filename .%s.\n", redir->filename);
		redir = redir->next;
	}
}

void	print_struct(t_cmd *cmd)
{
	// printf("Path :\n");
	// for(int i = 0; cmd->path[i]; i++)
	// 	printf("%s\n", cmd->path[i]);
	// printf("Cmd with args :\n");
	// printf("ENV : \n");
	// for (int i = 0; cmd->env_ms[i]; i++)
	// 	printf("%s\n", cmd->env_ms[i]);
	if (cmd->cmd)
	{
		for(int i = 0; cmd->cmd[i]; i++)
		{
			if (i == 0)
				printf("\ncmd : %s\n", cmd->cmd[i]);
			else
				printf("args : %s\n", cmd->cmd[i]);
		}
	}
	else
		printf("\ncmd->cmd : (null)\n");
	printf("Is that a builtin : %d\n", cmd->builtin);
	printf("Cmd path : %s\n", cmd->path_cmd);
	print_redir(cmd->redir);
}

void	print_linked_list(t_cmd *pipe)
{
	for(int i = 0; pipe; i++)
	{
		print_struct(pipe);
		pipe = pipe->next;
	}
}
