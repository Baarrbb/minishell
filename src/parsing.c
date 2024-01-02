/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:26:22 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/02 21:19:08 by bsuc             ###   ########.fr       */
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

static void	handle_error(char *s, t_cmd *pipex)
{
	printf(RED);
	printf(BOLD);
	printf("[ERROR]\n");
	printf(RESET);
	if (s)
		printf("%s\n", s);
	// free_struct(pipex);
	exit(EXIT_FAILURE);
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
	// free_path(del_path);
	return (path);
}

static char	*check_exist_cmd(char *cmd1, t_cmd *pipex)
{
	int		i;
	char	*full_cmd;
	char	**wo_param;

	full_cmd = 0;
	i = -1;
	wo_param = ft_split (cmd1, ' ');
	while (pipex->path[++i])
	{
		full_cmd = strjoin(full_cmd, pipex->path[i]);
		full_cmd = strjoin(full_cmd, "/");
		full_cmd = strjoin(full_cmd, wo_param[0]);
		if (access(full_cmd, X_OK) == 0)
		{
			// free_path(wo_param);
			return (full_cmd);
		}
		free(full_cmd);
		full_cmd = 0;
	}
	// free_path(wo_param);
	// handle_error("One of the command doesn't exist", pipex);
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
	cmd = (char *)ft_calloc(i, sizeof(char));
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

	i = -1;
	while (file_char(line[i]))
		count++;
	filename = ft_calloc(count + 1, sizeof(char));
	if (!filename)
		return (0);
	i = 0;
	while (file_char((int)*line))
	{
		filename[i] = (int)*line;
		line++;
		i++;
	}
	filename = ft_strtrim(filename, " ");
	redir->filename = filename;
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
	
	while (*line)
	{
		new = ft_calloc(1, sizeof(t_redir));
		if (!new)
			return ;
		ft_memset(new, 0, sizeof(t_redir));
		line = get_redir(new, line);
		line = ft_strtrim(line, ">< ");
		line = get_filename(new, line);
		new->next = 0;
		ft_lstadd_back(&redir, new);
		if (ft_strlen(line) == 0)
			break;
	}
	cmd->redir = redir;
}

static t_cmd	*init_cmd(char *line, char **envp, t_redir *redir)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->path = get_path(envp);
	if (ft_strchr(line, '<') || ft_strchr(line, '>') || ft_strchr(line, '&'))
		cmd->cmd = get_cmd(line); //gerer le &
	else
		cmd->cmd = ft_split(line, ' ');
	cmd->path_cmd = check_exist_cmd(cmd->cmd[0], cmd);

	if (ft_strnstr(line, ">", ft_strlen(line)) || ft_strnstr(line, "<", ft_strlen(line)))
		init_redir(cmd, redir, line);

	cmd->next = NULL;
	return (cmd);
}

int main(int ac, char **av, char **envp)
{
	char	*line;
	char	**command;
	t_cmd	*cmd;
	t_cmd	**pipe;
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
			print_struct(cmd);
			printf("je seg fault ???\n");
			// if (!cmd)
				// free_list(pipe);
			// pipe = ft_lst_addback()
		}
		free(line);
		free_char_tab(command);
		free_list(&cmd);
		// free_list(&redir);
		command = 0;
		redir = 0;
		i = -1;
	}
	return (0);
}

void	print_redir(t_redir *redir)
{
	for (int i = 0; redir; i++)
	{
		printf("out %d end %d\n", redir->out, redir->out_end);
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
		printf("%s\n", cmd->cmd[i]);
	printf("Is that a builtin : %d\n", cmd->builtin);
	printf("Cmd path : %s\n", cmd->path_cmd);
	printf("Back ground ? %d \n\n", cmd->background);
	print_redir(cmd->redir);
}
