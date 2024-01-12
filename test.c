
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

# define BOLD "\x1b[1m"
# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define ERROR_MSG "bash: syntax error near unexpected token "



typedef struct s_redir
{
	int		out;
	int		out_end;
	int		in;
	int		in_read;
	char	*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**path;
	char			**cmd;
	int				builtin;
	char			*path_cmd;
	int				exit_val;
	int				*cmd_quote; //tableau int pour quote 1 = sg 2 = db
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

void	print_linked_list(t_cmd *pipe);
void	print_struct(t_cmd *cmd);
void	print_redir(t_redir *redir);

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void	ft_bzero(void *s, size_t n)
{
	char	*str;
	size_t	i;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ret;

	if (size > 0 && nmemb > (SIZE_MAX / size))
		return (NULL);
	ret = malloc(nmemb * size);
	if (!ret)
		return (NULL);
	ft_bzero(ret, nmemb * size);
	return (ret);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len || !len)
	{
		ret = (char *)ft_calloc(1, sizeof(char));
		if (!ret)
			return (NULL);
		return (ret);
	}
	if (len >= s_len)
		len = s_len - start;
	else if (len > s_len - start)
		len--;
	ret = (char *)ft_calloc((len + 1), sizeof(char));
	if (!ret)
		return (NULL);
	s += start + (len - 1);
	ret += len;
	while (len--)
		*--ret = *s--;
	return (ret);
}

int	ft_isprint(int c)
{
	if (c >= ' ' && c <= '~')
		return (1);
	return (0);
}

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		*str++ = (unsigned char)c;
		i++;
	}
	return (s);
}

static int	count_words(char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] != c && s[i])
		{
			count++;
			i++;
		}
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static char	**malloc_free(char	**tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static char	*malloc_word(char *s, char c)
{
	int		i;
	int		j;
	char	*word;

	i = 0;
	j = 0;
	while (s[i] != c && s[i])
		i++;
	word = (char *)malloc((i + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (j < i)
	{
		word[j++] = (char)*s++;
	}
	word[j] = '\0';
	return (word);
}

static char	**insert_words(char **words, char *s, char c)
{
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && (char)*s == c)
			s++;
		if (*s && (char)*s != c)
		{
			words[i] = malloc_word(s, c);
			if (!words[i])
				return (malloc_free(words));
			i++;
		}
		while (*s && (char)*s != c)
			s++;
	}
	words[i] = NULL;
	return (words);
}

char	**ft_split(const char *s, char c)
{
	int		count;
	char	**split;

	if (!s)
		return (NULL);
	count = count_words((char *)s, c);
	split = (char **)malloc((count + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split = insert_words(split, (char *)s, c);
	return (split);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if ((unsigned char)*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (!(unsigned char)c)
		return ((char *)s);
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(s) + 1;
	dest = (char *)malloc(len * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t		i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && i < n - 1 && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	count;
	size_t	llen;

	if (!big && len == 0)
		return (NULL);
	llen = ft_strlen(little);
	if (llen == 0)
		return ((char *)big);
	count = 0;
	while (*big && count < len)
	{
		if ((ft_strncmp((char *)big, (char *)little, llen) == 0)
			&& count + llen <= len)
		{
			return ((char *)big);
		}
		big++;
		count++;
	}
	return (0);
}

static int	ischarset(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ncharset(char const *s1, char const *set)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while (ischarset(s1[i], (char *)set) && s1[i])
	{
		count++;
		i++;
	}
	if (!s1[i])
		return (count);
	i = ft_strlen(s1) - 1;
	while (ischarset(s1[i], (char *)set) && i != 0)
	{
		count++;
		i--;
	}
	return (count);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	int		i;
	int		len;
	char	*s;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	len = ft_strlen(s1) - ncharset(s1, set);
	while (ischarset((char)s1[i], (char *)set))
		i++;
	s = ft_substr(s1, i, len);
	return (s);
}

void	ft_lstadd_back(t_redir **lst, t_redir *new)
{
	t_redir	*tmp;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	ft_lstadd_back_bis(t_cmd **lst, t_cmd *new)
{
	t_cmd	*tmp;

	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	return_free(char *tofree, int ret)
{
	free(tofree);
	return (ret);
}

void	free_char_tab(char **tab)
{
	int	i;

	i = -1;
	if (*tab)
	{
		while (tab[++i])
			free(tab[i]);
	}
	if (tab)
		free(tab);
}

static void	free_redir(t_redir **redir)
{
	t_redir	*tmp;

	while (*redir)
	{
		tmp = (*redir)->next;
		free((*redir)->filename);
		free(*redir);
		*redir = tmp;
	}
	*redir = 0;
}

void	free_struct(t_cmd *cmd)
{
	if (cmd->path)
		free_char_tab(cmd->path);
	if (cmd->cmd)
		free_char_tab(cmd->cmd);
	free_redir(&(cmd->redir));
	free(cmd->path_cmd);
	free(cmd);
}

void	free_list(t_cmd **list)
{
	t_cmd	*tmp;

	if (list)
	{
		while (*list)
		{
			if ((*list)->path)
				free_char_tab((*list)->path);
			if ((*list)->cmd)
				free_char_tab((*list)->cmd);
			free_redir(&((*list)->redir));
			free((*list)->path_cmd);
			tmp = (*list)->next;
			free(*list);
			*list = tmp;
		}
		*list = 0;
	}
}

static char	**get_cmd_w_redir(char *line);
static t_cmd	*init_cmd(char *line, char **envp, t_redir *redir);
static	void	fill_cmd(t_cmd *cmd, char *line, char **envp);

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

char	*trim_space(char *line)
{
	printf("trim_pace\n");
	if (line)
	{
		while (*line == ' ' || *line == '	')
			line++;
	}
	return (line);
}

int	is_space(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

static char **copy_env(char **envp)
{
	int		i;
	char	**cpy_env;

	i = 0;
	if (!envp[0])
		return (0);
	while (envp[i])
		i++;
	cpy_env = ft_calloc(i + 1, sizeof(char *));
	if (!cpy_env)
		return (0);
	i = -1;
	while (envp[++i])
		cpy_env[i] = ft_strdup(envp[i]);
	return (cpy_env);
}

static char	**get_path(char **envp)
{
	printf("get path\n");
	char	**path;
	char	**del_path;
	int		i;

	i = -1;
	path = 0;
	del_path = 0;
	if(!envp)
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
	free_char_tab(del_path);
	return (path);
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

static int	check_quote(char *line)
{
	printf("check_quote\n");
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

static char	**get_cmd(char *line)
{
	printf("get_cmd\n");
	char	**tmp;
	char	**quote;

	tmp = 0;
	quote = 0;
	if (!ft_strchr(line, '\'') && !ft_strchr(line, '\"'))
	{
		if (ft_strchr(line, '<') || ft_strchr(line, '>'))
			return (get_cmd_w_redir(line));
		else
			return (ft_split(line, ' '));
	}
	else
		return (args_w_quote(line));
}

static char	**get_cmd_w_redir(char *line)
{
	printf("get_cmd_w_redir\n");
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

//tous les caracteres speciaux sauf space tab newline
// metacharacters : | & ; ( ) < > ; $
static int	is_spe_char(int c)
{
	// printf("is_spe_char\n");
	// ajouter  c == '$' ????
	if (c == '>' || c == '<' || c == '|') //|| c == '\'' || c == '\"'
		return (1);
	return (0);
}

static int	file_char(int c)
{
	// printf("file_char\n");
	if (ft_isprint(c) && !is_spe_char(c) && c != ' ')
		return (1);
	return (0);
}

static void	error_syntax(char *line)
{
	printf("error_syntax\n");
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

static char	*test_filename(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while(!is_space(line[i]) && line[i])
	{
		if (line[i] == '\'' && line[i])
		{
			i++;
			while (line[i] != '\'')
				i++;
		}
		else if (line[i] == '\"' && line[i])
		{
			i++;
			while (line[i] != '\"' && line[i])
				i++;
		}
		else
		{
			while (file_char(line[i]))
			{
				count++;
				i++;
			}
			if (count == 0)
			{
				error_syntax(line);
				return (0);
			}
		}
	}
	return(ft_substr(line, 0, i));
}

static char	*get_filename(t_redir *redir, char *line)
{
	printf("get_filename\n");
	int		i;
	int		count;
	char	*filename;
	char	*filetrim;

	i = -1;
	count = 0;
	if (!redir->out && !redir->in && !redir->out_end && !redir->in_read)
		return (0);
	if (redir->out || redir->in)
		line++;
	else if (redir->out_end || redir->in_read)
		line += 2;
	while (line[++i] == ' ' && *line)
		line++;
	i = -1;
	// while (file_char(line[++i]))
	// 	count++;
	// if (count == 0)
	// {
	// 	error_syntax(line);
	// 	return (0);
	// }
	// filename = ft_calloc((count + 1), sizeof(char));
	// if (!filename)
	// 	return (0);
	// i = 0;
	// while (file_char((int)*line))
	// {
	// 	filename[i] = (int)*line;
	// 	line++;
	// 	i++;
	// }
	// filetrim = ft_strtrim(filename, " ");
	// free(filename);
	// redir->filename = filetrim;
	redir->filename = test_filename(line);
	line += ft_strlen(redir->filename);
	if (*line == 0)
		return (0);
	return (line);
}

static char	*get_redir(t_redir *cmd, char *line)
{
	printf("get_redir\n");
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

static char	*get_new_arg(char *line)
{
	printf("get_new_arg\n");
	int		i;
	char	*arg;

	line = trim_space(line);
	i = 0;
	while (!is_spe_char(line[i]) && line[i] != ' ' && line[i])
		i++;
	arg = ft_calloc(i + 1, sizeof(char));
	if (!arg)
		return (0);
	i = -1;
	while (!is_spe_char(line[++i]) && line[i] != ' ' && line[i])
		arg[i] = line[i];
	return (arg);
}

static	t_cmd	*init_redir(t_cmd *cmd, t_redir *redir, char *line, char **envp)
{
	printf("init_redir\n");
	t_redir	*new;
	char	*linetrim;
	char	*tmp;
	int		i;

	linetrim = ft_strdup(line);
	tmp = linetrim;
	while (*linetrim)
	{
		i = -1;
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
		linetrim = get_filename(new, linetrim);
		if (!new->filename && !cmd)
		{
			free(new);
			free(tmp);
			return (0);
		}
		if (!new->filename)
			break ;
		new->next = 0;
		ft_lstadd_back(&redir, new);
		if (ft_strlen(linetrim) == 0)
			break ;
		linetrim = trim_space(linetrim);
		if (!cmd->cmd && linetrim[0] != '>' && linetrim[0] != '<')
		{
			fill_cmd(cmd, linetrim, envp);
			linetrim += ft_strlen(cmd->cmd[0]);
		}
		else if (linetrim[0] != '>' && linetrim[0] != '<' && ft_strlen(linetrim))
		{
			t_cmd *new_cmd;

			new_cmd = ft_calloc(1, sizeof(t_cmd));
			if (!new_cmd)
				return (0);
			int	i = 0;
			while (cmd->path[i])
				i++;
			new_cmd->path = ft_calloc(i + 1, sizeof(char *));
			if (!new_cmd->path)
				return (0);
			i = -1;
			while (cmd->path[++i])
				new_cmd->path[i] = ft_strdup(cmd->path[i]);
			i = 0;
			while (cmd->cmd[i])
				i++;
			new_cmd->cmd = ft_calloc((i + 2), sizeof(char *));
			if (!new_cmd->cmd)
				return (0);
			i = -1;
			while (cmd->cmd[++i])
				new_cmd->cmd[i] = ft_strdup(cmd->cmd[i]);
			new_cmd->cmd[i] = get_new_arg(linetrim);
			linetrim += ft_strlen(new_cmd->cmd[i]);
			new_cmd->builtin = cmd->builtin;
			if (cmd->path_cmd)
				new_cmd->path_cmd = ft_strdup(cmd->path_cmd);
			free_struct(cmd);
			cmd = new_cmd;
		}
	}
	cmd->redir = redir;
	free(tmp);
	return (cmd);
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

static	void	fill_cmd(t_cmd *cmd, char *line, char **envp)
{
	cmd->path = get_path(envp);
	cmd->cmd = get_cmd(line);
	cmd->path_cmd = check_exist_cmd(cmd->cmd[0], cmd);
	is_builtin(cmd);
}

static t_cmd	*init_cmd(char *line, char **envp, t_redir *redir)
{
	printf("init_cmd\n");
	t_cmd	*cmd;
	int		len;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (0);
	ft_memset(cmd, 0, sizeof(t_cmd));
	if (line[0] != '<' && line[0] != '>')
		fill_cmd(cmd, line, envp);
	len = ft_strlen(line);
	if (ft_strnstr(line, ">", len) || ft_strnstr(line, "<", len))
	{	
		cmd = init_redir(cmd, redir, line, envp);
		if (!cmd)
		{
			free_struct(cmd);
			return (0);
		}
	}
	return (cmd);
}

static t_cmd *check_grammar_line(t_redir *redir, t_cmd *cmd, char *line, char **envp)
{
	printf("check_grammar_line\n");
	char	*linetrim;

	linetrim = ft_strtrim(line, " 	");
	if (ft_strlen(linetrim) == 0)
	{
		free(linetrim);
		return (0);
	}
	if (linetrim[0] == '&')
	{
		if (linetrim[1] == '&')
			printf("%s `%s\'\n", ERROR_MSG, "&&");
		else
			printf("%s `%s\'\n", ERROR_MSG, "&");
		free(linetrim);
		return (0);
	}
	if (!check_quote(line))
	{
		printf("bash: syntax error quote expected\n");
		free(linetrim);
		return (0);
	}
	cmd = init_cmd(linetrim, envp, redir);
	free(linetrim);
	return (cmd);
}

static int	count_pipe(char *line)
{
	printf("count_pipe\n");
	int	count;

	count = 0;
	while (*line)
	{
		if (*line == '|')
			count++;
		line++;
	}
	return (count);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	char	**command;
	t_cmd	*cmd;
	t_cmd	*pipe;
	t_redir	*redir;
	int		i;
	int		yesh_pipe;
	char	**cpy_env;

	(void)ac;
	(void)av;
	i = -1;
	pipe = 0;
	cmd = 0;
	redir = 0;
	cpy_env = copy_env(envp);
	while (1)
	{
		line = readline("Minishell $ ");
		rl_on_new_line();
		// if (!line)
			//exit
		if (line[0] != ' ' && line[0] != 0)
			add_history(line);
		yesh_pipe = count_pipe(line);
		command = ft_split(line, '|');
		while (command[++i])
		{
			cmd = check_grammar_line(redir, cmd, command[i], cpy_env);
			if (cmd)
				ft_lstadd_back_bis(&pipe, cmd);
			else
			{
				free_list(&pipe);
				break;
			}
		}
		if (yesh_pipe && i < yesh_pipe + 1)
		{
			printf("%s `%s\'\n", ERROR_MSG, "|");
			free(line);
			free_char_tab(command);
			free_list(&pipe);
		}
		else
		{
			print_linked_list(pipe);
			free(line);
			free_char_tab(command);
			free_list(&pipe);
		}
		command = 0;
		cmd = 0;
		i = -1;
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
