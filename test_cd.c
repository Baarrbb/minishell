
# include "parsing.h"

static int	get_nb_args(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->cmd && cmd->cmd[0])
	{
		while (cmd->cmd[i])
			i++;
	}
	return (i);
}

static void	move_cd(char *path)
{
	if (chdir(path) != 0)
	{
		if (errno == EACCES)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));//ok mauvaise permission
		if (errno == EFAULT)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == EIO)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == ELOOP)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == ENAMETOOLONG)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno)); // ok
		if (errno == ENOENT)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno)); //ok repertoire n'existe pas "jexistepas"
		if (errno == ENOMEM)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
		if (errno == ENOTDIR)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));//ok pas un repertoire "/bin/bash"
		if (errno == EBADF)
			printf("%s %s: %s\n", ERROR_CD, path, strerror(errno));
	}
}

void	our_cd(t_cmd *cmd, char **env)
{
	int		nb_args;
	char	*home;

	nb_args = get_nb_args(cmd);
	home = 0;
	if (nb_args > 2)
	{
		printf("%s too many arguments\n", ERROR_CD);
		return ;
	}
	if (nb_args == 1 || !ft_strncmp(cmd->cmd[1], "~", ft_strlen(cmd->cmd[1])))
	{
		// home = get_ourenv("HOME", env);
		home = getenv("HOME");
		if (!home && nb_args == 1)
			printf("%s HOME not set\n", ERROR_CD);
		else
			move_cd(cmd->cmd[1]);
	}
	else
		move_cd(cmd->cmd[1]);
}


int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));

	cmd->cmd = ft_calloc(3, sizeof(char *));
	cmd->cmd[0] = ft_strdup("cd");

	char longPath[PATH_MAX];
	ft_memset(longPath, 'a', PATH_MAX - 1);
	longPath[PATH_MAX - 1] = '\0'; // erreur name too long

	cmd->cmd[1] = ft_strdup("~");

	our_cd(cmd, envp);
}