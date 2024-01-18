/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsingbis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:36:10 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/18 21:05:30 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

char	*trim_space(char *line)
{
	if (line)
	{
		while (*line == ' ' || *line == '	')
			line++;
	}
	return (line);
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
	// printf("check_quote\n");
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
			continue ;
		}
	}
	return (args);
}

static int	error_syntax(char **args, char *line)
{
	char	*token;

	token = 0;
	if ((!line || line[0] == 0) && !args[1])
		token = "newline";
	else if (line[0] == '|')
		token = "|";
	else if ((line[0] == '>' && line[1] == '>')
		|| (args[1] && args[1][0] == '>' && args[1][1] == '>'))
		token = ">>";
	else if ((line[0] == '<' && line[1] == '<')
		|| (args[1] && args[1][0] == '<' && args[1][1] == '<'))
		token = "<<";
	else if (line[0] == '>' || (args[1][0] == '>'))
		token = ">";
	else if (line[0] == '<' || (args[1][0] == '<'))
		token = "<";
	if (token)
	{
		printf("%s`%s\'\n", ERROR_MSG, token);
		return (0);
	}
	return (1);
}

static int	check_syntax_redir(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (args[i][0] == '<' && args[i][1] == '>')
		{
			printf("%s`newline\'\n", ERROR_MSG);
			return (0);
		}
		else if (args[i][0] == '>' && args[i][1] == '>')
		{
			if (!error_syntax(&args[i], &args[i][2]))
				return (0);
		}
		else if (args[i][0] == '<' && args[i][1] == '<')
		{
			if (!error_syntax(&args[i], &args[i][2]))
				return (0);
		}
		else if (args[i][0] == '>' && args[i][1] != '>')
		{
			if (!error_syntax(&args[i], &args[i][1]))
				return (0);
			
		}
		else if (args[i][0] == '<' && args[i][1] != '<')
		{
			if (!error_syntax(&args[i], &args[i][1]))
				return (0);
		}
	}
	return (1);
}

static int	check_syntax_pipe(char **args, int size)
{
	if (args[0][0] == '|' && args[0][1] == '|')
	{
		printf("%s`||\'\n", ERROR_MSG);
		return (0);
	}
	else if (args[0][0] == '|')
	{
		printf("%s`|\'\n", ERROR_MSG);
		return (0);
	}
	else if (args[size - 1][0] == '|')
	{
		printf("%s`|\'\n", ERROR_MSG);
		return (0);
	}
	return (1);
}

static int	check_syntax(char **args, int size)
{
	return (check_syntax_redir(args) && check_syntax_pipe(args, size));
}

/************************************************************************************************/

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

static void	add_shlvl(char **envp)
{
	int		i;
	int		j;
	char	*new;

	i = -1;
	while (envp[++i])
	{
		if (!ft_strncmp(envp[i], "SHLVL", ft_strlen("SHLVL")))
		{
			j = 0;
			while (envp[i][j] != '=')
				j++;
			j += 1;
			if (ft_atoi(envp[i] + j))
			{
				new = ft_itoa(ft_atoi(envp[i] + j) + 1);
				free(envp[i]);
				envp[i] = 0;
				envp[i] = strjoin(ft_strdup("SHLVL="), new);
				free(new);
			}
		}
	}
}

static char **copy_env_null(void)
{
	char	**new_env;
	char	pwd[PATH_MAX];

	getcwd(pwd, PATH_MAX);
	new_env = ft_calloc(4, sizeof(char *));
	if (!new_env)
		return (0);
	new_env[0] = strjoin(new_env[0], "PWD=");
	new_env[0] = strjoin(new_env[0], pwd);
	new_env[1] = strjoin(new_env[1], "SHLVL=");
	new_env[1] = strjoin(new_env[1], "1");
	new_env[2] = strjoin(new_env[2], "_=");
	new_env[2] = strjoin(new_env[2], "./minishell");
	return (new_env);
}

static char	**copy_env(char **envp)
{
	int		i;
	char	**cpy_env;

	i = 0;
	if (!envp[0])
	{
		cpy_env = copy_env_null();
		add_shlvl(cpy_env);
		return (cpy_env);
	}
	while (envp[i])
		i++;
	cpy_env = ft_calloc(i + 1, sizeof(char *));
	if (!cpy_env)
		return (0);
	i = -1;
	while (envp[++i])
		cpy_env[i] = ft_strdup(envp[i]);
	add_shlvl(cpy_env);
	return (cpy_env);
}

/************************************************************************************************/

static	void	init_redir(t_cmd **cmd, char **args, int type)
{
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

static t_cmd	*fill_cmd(t_cmd **pipe, char **args)
{
	t_cmd	*cmd;
	int	i;

	i = -1;
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (0);
	ft_memset(cmd, 0, sizeof(t_cmd));
	while (args[++i])
	{
		if (!ft_strncmp(args[i], ">", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 1);
			i++;
		}
		else if (!ft_strncmp(args[i], ">>", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 2);
			i++;
		}
		else if (!ft_strncmp(args[i], "<", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 3);
			i++;
		}
		else if (!ft_strncmp(args[i], "<<", ft_strlen(args[i])))
		{
			init_redir(&cmd, &args[i], 4);
			i++;
		}
		// else if (!ft_strncmp(args[i], "|", ft_strlen(args[i])))
		// {
		// 	ft_lstadd_back_bis(pipe, fill_cmd(pipe, &args[i]));
		// 	i++;
		// }
		// else
		// {
			
		// }
	}
	return (cmd);
}

static t_cmd	*check_line(char *line, t_cmd **pipe)
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
	for(int i = 0; args[i]; i++)
		printf("%d : .%s.\n", i, args[i]);
	printf("\n");
	if(!check_syntax(args, size))
		return (0);
	return (fill_cmd(pipe, args));
}


int	main(int ac, char **av, char **envp)
{
	char	*line;
	// char	**command;
	// t_cmd	*cmd;
	t_cmd	*pipe;
	// t_redir	*redir;
	// int		i;
	// char	**cpy_env;

	(void)ac;
	(void)av;
	// i = -1;
	pipe = 0;
	// cmd = 0;
	// redir = 0;
	// cpy_env = copy_env(envp);
	while (1)
	{
		line = readline("Minishell $ ");
		rl_on_new_line();
		// if (!line)
			//exit
		if (line[0] != ' ' && line[0] != 0)
			add_history(line);
		pipe = check_line(line, &pipe);
		print_linked_list(pipe);
		// command = 0;
		// cmd = 0;
		// i = -1;
	}
	return (0);
}


void	print_redir(t_redir *redir)
{
	if (redir)
	{
		for (int i = 0; redir; i++)
		{
			printf("out %d end %d\n", redir->out, redir->out_end);
			printf("in %d read %d\n", redir->in, redir->in_read);
			printf("Filename .%s.\n", redir->filename);
			redir = redir->next;
		}
	}

}

void	print_struct(t_cmd *cmd)
{
	// printf("Path :\n");
	// for(int i = 0; cmd->path[i]; i++)
	// 	printf("%s\n", cmd->path[i]);
	// printf("Cmd with args :\n");
	if (!cmd)
	{
		printf("NULL\n");
		return ;
	}
	if (cmd->cmd)
	{
		for (int i = 0; cmd->cmd[i]; i++)
		{
			if (i == 0)
				printf("\ncmd : .%s.\n", cmd->cmd[i]);
			else
				printf("args : .%s.\n", cmd->cmd[i]);
		}
	}
	else
		printf("\ncmd->cmd : (null)\n");
	printf("Is that a builtin : %d\n", cmd->builtin);
	printf("Cmd path : %s\n", cmd->path_cmd);
	if (cmd->redir)
	{
		print_redir(cmd->redir);
	}
}

void	print_linked_list(t_cmd *pipe)
{
	while (pipe)
	{
		print_struct(pipe);
		pipe = pipe->next;
	}
}
