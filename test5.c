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

// static void	get_arg(t_cmd *cmd, char *arg, char c, int i)
// {
// 	char	*new;
// 	char	*sub;
// 	int		j;
// 	new = 0;
// 	sub = 0;
// 	j = 0;
// 	while (arg[j] != '\'' && arg[j] != '\"')
// 		j++;
// 	while (*arg)
// 	{
// 		while(arg[j] != c && arg[j])
// 			j++;
// 		sub = ft_substr(arg, 0, j);
// 		new = strjoin(new, sub);
// 		free(sub);
// 		j++;
// 		arg += j;
// 		j = 0;
// 	}
// 	free(cmd->cmd[i]);
// 	cmd->cmd[i] = new;
// }

// static void remove_quote_args(t_cmd *cmd)
// {
// 	int		i;
// 	int		j;
// 	char	*new;
// 	char	*tmp;
// 	i = 0;
// 	j = 0;
// 	new = 0;
// 	while (cmd->cmd[i])
// 	{
// 		tmp = cmd->cmd[i];
// 		if (ft_strchr(tmp, '\'') || ft_strchr(tmp, '\"'))
// 		{
// 			while (tmp[j] != '\'' && tmp[j] != '\"')
// 				j++;
// 			if (tmp[j] == '\"')
// 				get_arg(cmd, tmp, '\"', i);
// 			else
// 				get_arg(cmd, tmp, '\'', i);
// 		}
// 		i++;
// 	}
// }


static void	get_file(t_redir *redir, char *filename, char c)
{
	char	*new;
	char	*sub;
	int		j;

	new = 0;
	sub = 0;
	j = 0;
	while (filename[j] != '\'' && filename[j] != '\"')
		j++;
	while (*filename)
	{
		while(filename[j] != c && filename[j])
			j++;
		sub = ft_substr(filename, 0, j);
		new = strjoin(new, sub);
		free(sub);
		j++;
		filename += j;
		j = 0;
	}
	free(redir->filename);
	redir->filename = new;
}

static void remove_quote_file(t_redir *redir)
{
	int		i;
	int		j;
	char	*new;
	char	*tmp;

	i = 0;
	j = 0;
	new = 0;
	while (redir)
	{
		tmp = redir->filename;
		if (ft_strchr(tmp, '\'') || ft_strchr(tmp, '\"'))
		{
			while (tmp[j] != '\'' && tmp[j] != '\"')
				j++;
			if (tmp[j] == '\"')
				get_file(redir, tmp, '\"');
			else
				get_file(redir, tmp, '\'');
		}
		redir = redir->next;
	}
}

//  "sed -n   p'sdsdf'd  kk'dsf\"sdf df' 'dsf  df'sdd  \"s/,   $/./p\"   "

// cat /etc/passwd | cut -d: -f1 | sed -n '2~2p' | rev | sort -r | sed -n "$FT_LINE1","$FT_LINE2"p | tr '\n' ',' | sed -n  | sed -n 's/, $/./p' | tr -d '\n'
int main()
{
	t_redir *redir = ft_calloc(1, sizeof(t_redir));
	t_redir *redirbis = ft_calloc(1, sizeof(t_redir));
	redir->next = redirbis;
	redir->filename = ft_strdup("ou\"i\"");
	redirbis->filename = ft_strdup("\"bonjour\"");
	printf("DANS redir1 : %s\n", redir->filename);
	printf("DANS redir2 : %s\n", redir->next->filename);
	remove_quote_file(redir);
	printf("%s\n", redir->filename);
	printf("%s\n", redirbis->filename);
}

// bon'jour'k'' = bonjourk