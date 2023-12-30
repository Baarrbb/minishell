/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:26:22 by bsuc              #+#    #+#             */
/*   Updated: 2023/12/31 00:27:42 by bsuc             ###   ########.fr       */
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

static char	*get_file(char *line, int len)
{
	int		i;
	char	*file;
	
	i = 0;
	file = ft_calloc(len + 1, sizeof(char));
	if (!file)
		return (NULL);
	i = -1;
	while (++i < len)
		file[i] = line[i];
	file[i] = 0;
	return (file);
}

static int	count_file(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	line = ft_strtrim(line, " ");
	while (line[i])
	{
		if (line[i] == '>' && line[i + 1] != '>')
			count++;
		if (line[i] == '>' && line[i + 1] == '>')
			i++;
		i++;
	}
	return (count);
}

static char	**get_redir_out(char *line)
{
	char	**files;
	char	*out;
	int		i;
	int		count;
	int		j;

	files = 0;
	j = 0;
	out = ft_strnstr(line, ">", ft_strlen(line));
	i = 0;
	count = 0;
	files = ft_calloc(count_file(out) + 1, sizeof(char *));
	if (!files)
		return (NULL);
	while(*out)
	{
		if (*out == '>' && *(out + 1) != '>')
		{
			out = ft_strtrim(out, "> ");
			while (out[i] && out[i] != '>' && out[i] != '<' && out[i] != ' ')
			{
				count++;
				i++;
			}
			files[j] = get_file(out, count);
			count = 0;
			j++;
			i = 0;
		}
		if (*out == '>' && *(out + 1) == '>')
			out++;
		out++;
	}
	files[j] = 0;
	return (files);
}

static t_cmd	*init_cmd(char *line, char **envp)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->path = get_path(envp);
	if (ft_strchr(line, '<') || ft_strchr(line, '>'))
		cmd->cmd = get_cmd(line);
	else
		cmd->cmd = ft_split(line, ' ');
	for(int i = 0; cmd->cmd[i]; i++)
		printf(".%s.\n", cmd->cmd[i]);
	cmd->path_cmd = check_exist_cmd(cmd->cmd[0], cmd);
	printf("path_cmd %s\n", cmd->path_cmd);

	if (ft_strnstr(line, ">", ft_strlen(line)))
		cmd->redir_out = get_redir_out(line);
	// if (ft_strnstr(line, ">>", ft_strlen(line)))
	// 	// cmd->redir_out_end = get_redir_out_end(line);
	// if (ft_strchr(line, '<'))
	// if (ft_strnstr(line, "<<", 2))

	for (int i = 0; cmd->redir_out[i]; i++)
		printf("redir out : .%s.\n", cmd->redir_out[i]);
	cmd->next = NULL;
	return (cmd);
}

int main(int ac, char **av, char **envp)
{
	char	*line;
	char	**command;
	t_cmd	*cmd;
	t_cmd	**pipe;
	int		i;

	(void)ac;
	(void)av;
	i = -1;
	pipe = 0;
	cmd = 0;
	while (1)
	{
		line = readline("Minishell $ ");
		rl_on_new_line();
		if (line[0] != ' ')
			add_history(line);
		command = ft_split(line, '|');
		while (command[++i])
		{
			printf("cmd %d : %s\n", i, command[i]);
			cmd = init_cmd(command[i], envp);
			// if (!cmd)
				// free_list(pipe);
			// pipe = ft_lst_addback()
		}
		free(line);
		free_char_tab(command);
		free_list(&cmd);
		command = 0;
		i = -1;
	}
	return (0);
}
