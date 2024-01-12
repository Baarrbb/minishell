/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:40:10 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/12 12:55:37 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>


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

void	heredoc(void)
{
	int pipeheredoctest[2];
	char	*delimiter = "testss";
	char *line;
	
	if (pipe(pipeheredoctest) < 0)
	{
		perror("fail pipe");
		return ;
	}
	int pid = fork();
	/*if (pid == -1)
	{
		perror("fork");
		exit(127);
	}*/
	if (pid == 0)
	{
		close(pipeheredoctest[0]);
		//dup2(pipeheredoctest[1], 1);
		while (1)
		{
			line = readline("testheredoc : ");
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			{
				if (delimiter[ft_strlen(delimiter)] )
				printf("break");
				free(line);
				break;
			}
			printf("%s", line);
		}
		close(pipeheredoctest[1]);
		exit(0);
	}
	if (pid > 0)
		dup2(pipeheredoctest[0], 0);
	int status;
	waitpid(pid, &status, 0);
}
int main(void)
{
	heredoc();
	printf("");
}