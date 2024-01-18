/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:36:10 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 01:04:05 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Partie je fais un double tableau en separant les mots des tokens
 * et verification syntaxe
*/

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

static int	is_spe_char(int c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

static int	check_quote(char *line)
{
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

static int	get_next_quote(char *line, int i)
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
	return (i);
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
			i = get_next_quote(line, i);
		if (i - 1 >= 0 && !is_space(line[i - 1]) && !is_spe_char(line[i - 1]))
			count++;
		if (is_spe_char(line[i]))
		{
			while (is_spe_char(line[i]) && line[i])
				i++;
			count++;
		}
		while (is_space(line[i]) && line[i])
			i++;
	}
	return (count);
}

// cat test > ">> out|" | test 1 "@ #" >>>>> >> > <
static char	**fill_args(char **args, char *line)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (*line && line[i])
	{
		while (is_space(*line) && *line)
			line++;
		while (!is_space(line[i]) && !is_spe_char(line[i]) && line[i])
			i = get_next_quote(line, i);
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
			while (is_spe_char(line[i]))
				i++;
			args[j] = ft_substr(line, 0, i);
			j++;
			line += i;
			i = 0;
		}
	}
	return (args);
}



/**
* Partie liste chainee et redirection
*/

static char	**get_path(char **envp)
{
	printf("get path\n");
	char	**path;
	char	**del_path;
	int		i;

	i = -1;
	path = 0;
	del_path = 0;
	if (!envp)
		return (0);
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH", 4))
		{
			del_path = ft_split(envp[i], '=');
			path = ft_split(del_path[1], ':');
			break ;
		}
	}
	if (del_path)
		free_char_tab(del_path);
	return (path);
}

static void	is_builtin(t_cmd *cmd)
{
	printf("id_builtin\n");
	if (!cmd->cmd)
		return ;
	if (!ft_strncmp(cmd->cmd[0], "echo", ft_strlen(cmd->cmd[0])))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "cd", ft_strlen(cmd->cmd[0])))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "pwd", ft_strlen(cmd->cmd[0])))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "export", ft_strlen(cmd->cmd[0])))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "unset", ft_strlen(cmd->cmd[0])))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "env", ft_strlen(cmd->cmd[0])))
		cmd->builtin = 1;
	else if (!ft_strncmp(cmd->cmd[0], "exit", ft_strlen(cmd->cmd[0])))
		cmd->builtin = 1;
}

static char	*check_exist_cmd(char *cmd1, t_cmd *cmd)
{
	printf("check_exist_cmd\n");
	int		i;
	char	*full_cmd;
	char	**wo_param;

	if (!cmd->path)
		return (0);
	full_cmd = 0;
	i = -1;
	wo_param = ft_split(cmd1, ' ');
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

static	void	init_redir(t_cmd **cmd, char **args, int type)
{
	printf("init_redir\n");
	t_redir	*new;
	t_redir	*redir;

	redir = (*cmd)->redir;
	new = ft_calloc(1, sizeof(t_redir));
	if (!new)
		return ;
	ft_memset(new, 0, sizeof(t_redir));
	if (type == 1)
		new->out = 1;
	else if (type == 2)
		new->out_end = 1;
	else if (type == 3)
		new->in = 1;
	else if (type == 4)
		new->in_read = 1;
	new->filename = ft_strdup(args[1]);
	ft_lstadd_back(&redir, new);
	(*cmd)->redir = redir;
}

static void	get_cmd(t_cmd **cmd, char **args)
{
	printf("get_cmd\n");
	int	nb_args;
	int	i;

	nb_args = 0;
	i = -1;
	while (args[++i] && ft_strncmp(args[i], ">", 1) && ft_strncmp(args[i], ">>", 2)
		&& ft_strncmp(args[i], "<", 1) && ft_strncmp(args[i], "<<", 2)
		&& ft_strncmp(args[i], "|", 1))
		nb_args++;
	(*cmd)->cmd = ft_calloc(nb_args + 1, sizeof(char *));
	if (!(*cmd)->cmd)
		return ;
	i = -1;
	while (args[++i] && ft_strncmp(args[i], ">", 1) && ft_strncmp(args[i], ">>", 2)
		&& ft_strncmp(args[i], "<", 1) && ft_strncmp(args[i], "<<", 2)
		&& ft_strncmp(args[i], "|", 1))
		(*cmd)->cmd[i] = ft_strdup(args[i]);
}

static void	add_arg(t_cmd **cmd, char **args)
{
	printf("add_arg\n");
	int		i;
	int		j;
	char	**new_cmd;

	i = 0;
	j = 0;
	while ((*cmd)->cmd[i])
		i++;
	while (args[j] && ft_strncmp(args[j], ">", 1) && ft_strncmp(args[j], ">>", 2)
		&& ft_strncmp(args[j], "<", 1) && ft_strncmp(args[j], "<<", 2)
		&& ft_strncmp(args[j], "|", 1))
		j++;
	new_cmd = ft_calloc(i + j + 1, sizeof(char *));
	if (!new_cmd)
		return ;
	i = -1;
	while ((*cmd)->cmd[++i])
		new_cmd[i] = ft_strdup((*cmd)->cmd[i]);
	j = -1;
	while (args[++j] && ft_strncmp(args[j], ">", 1) && ft_strncmp(args[j], ">>", 2)
		&& ft_strncmp(args[j], "<", 1) && ft_strncmp(args[j], "<<", 2)
		&& ft_strncmp(args[j], "|", 1))
	{
		new_cmd[i] = ft_strdup(args[j]);
		i++;
	}
	free_char_tab((*cmd)->cmd);
	(*cmd)->cmd = new_cmd;
}

static void	fill_cmd(t_cmd **pipe, char **args)
{
	t_cmd	*cmd;
	int	i;
	int	j;

	i = 0;
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return ;
	ft_memset(cmd, 0, sizeof(t_cmd));
	while (args[i] && ft_strncmp(args[i], "|", ft_strlen(args[i])))
	{
		j = 0;
		if (!ft_strncmp(args[i], ">", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 1);
			i += 2;
		}
		else if (!ft_strncmp(args[i], ">>", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 2);
			i += 2;
		}
		else if (!ft_strncmp(args[i], "<", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 3);
			i += 2;
		}
		else if (!ft_strncmp(args[i], "<<", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 4);
			i += 2;
		}
		else if (!cmd->cmd)
		{
			j = 0;
			get_cmd(&cmd, &args[i]);
			while (cmd->cmd[j])
			{
				i++;
				j++;
			}
			ft_lstadd_back_bis(pipe, cmd);
		}
		else if (cmd->cmd)
		{
			j = 0;
			add_arg(&cmd, &args[i]);
			while (args[i] && ft_strncmp(args[i], ">", 1) && ft_strncmp(args[i], ">>", 2)
				&& ft_strncmp(args[i], "<", 1) && ft_strncmp(args[i], "<<", 2)
				&& ft_strncmp(args[i], "|", 1))
				i++;
		}
	}
	if (args[i] && !ft_strncmp(args[i], "|", ft_strlen(args[i])))
		fill_cmd(pipe, &args[i + 1]);
}

static t_cmd	*fill_struct(t_cmd **pipe, char **args, char **envp)
{
	t_cmd	*tmp;

	fill_cmd(pipe, args);
	tmp = *pipe;
	while (tmp)
	{
		tmp->path = get_path(envp);
		is_builtin(tmp);
		tmp->path_cmd = check_exist_cmd(tmp->cmd[0], tmp);
		tmp = tmp->next;
	}
	free_char_tab(args);
	return (*pipe);
}

t_cmd	*check_line(char *line, t_cmd **pipe, char **envp)
{
	char	**args;
	int		size;

	if (!check_quote(line))
	{
		printf("minishell: syntax error quote expected\n");
		return (0);
	}
	size = get_nb_args(line);
	args = ft_calloc(size + 1, sizeof(char *));
	if (!args)
		return (0);
	fill_args(args, line);
	
	printf("NB ARGS %d\n", size);
	for(int i = 0; args[i]; i++)
		printf(".%s.\n", args[i]);


	if(!check_syntax(args, size))
		return (0);
	return(fill_struct(pipe, args, envp));
}
