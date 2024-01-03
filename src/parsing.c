/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:26:22 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/03 02:35:10 by bsuc             ###   ########.fr       */
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
static int is_spe_char(int c)
{
	if (c == '>' || c == '<' || c == '$' || c == '\'' || c == '\"' || c == '|')
		return (1);
	return (0);
}

static int	file_char(int c)
{
	if (ft_isprint(c) && !is_spe_char(c))
		return (1);
	return (0);
}

static char *get_filename(t_redir *redir, char *line)
{
	int		i;
	int		count;
	char	*filename;
	char	*filetrim;

	i = -1;
	count = 0;
	while (file_char(line[++i]))
		count++;
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

static	void	init_redir(t_cmd *cmd, t_redir *redir, char *line)
{
	t_redir	*new;
	char	*linetrim;
	char	*tmp;
	int		i;

	linetrim = ft_strdup(line);
	tmp = linetrim;
	while (*linetrim)
	{
		new = ft_calloc(1, sizeof(t_redir));
		if (!new)
			return ;
		ft_memset(new, 0, sizeof(t_redir));
		linetrim = get_redir(new, linetrim);
		i = -1;
		while (linetrim[++i] == ' ' || linetrim[i] == '<' || linetrim[i] == '>')
			linetrim++;
		linetrim = get_filename(new, linetrim);
		new->next = 0;
		ft_lstadd_back(&redir, new);
		if (ft_strlen(linetrim) == 0)
			break;
	}
	cmd->redir = redir;
	free(tmp);
}

static t_cmd	*init_cmd(char *line, char **envp, t_redir *redir)
{
	t_cmd	*cmd;
	char	*line_trim;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->path = get_path(envp);
	line_trim = ft_strtrim(line, " 	");
	if (line_trim[ft_strlen(line_trim) - 1] == '&')
	{
		cmd->background = 1;
		line_trim = ft_strtrim(line_trim, " 	&");
	}
	if (ft_strchr(line_trim, '<') || ft_strchr(line_trim, '>'))
		cmd->cmd = get_cmd(line_trim);
	else
		cmd->cmd = ft_split(line_trim, ' ');
	cmd->path_cmd = check_exist_cmd(cmd->cmd[0], cmd);
	if (ft_strnstr(line_trim, ">", ft_strlen(line_trim)) || ft_strnstr(line_trim, "<", ft_strlen(line_trim)))
		init_redir(cmd, redir, line_trim);
	cmd->next = NULL;
	free(line_trim);
	return (cmd);
}

int main(int ac, char **av, char **envp)
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
		if (line[0] != ' ')
			add_history(line);
		command = ft_split(line, '|');
		while (command[++i])
		{
			cmd = init_cmd(command[i], envp, redir);
			ft_lstadd_back_bis(&pipe, cmd);
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
	for(int i = 0; cmd->cmd[i]; i++)
	{
		if (i == 0)
			printf("cmd : %s\n", cmd->cmd[i]);
		else
			printf("args : %s\n", cmd->cmd[i]);
	}
	printf("Is that a builtin : %d\n", cmd->builtin);
	printf("Cmd path : %s\n", cmd->path_cmd);
	printf("Background ? %d \n\n", cmd->background);
	print_redir(cmd->redir);
}

void print_linked_list(t_cmd *pipe)
{
	for(int i = 0; pipe; i++)
	{
		print_struct(pipe);
		pipe = pipe->next;
	}
}
