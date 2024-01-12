/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 19:40:10 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/12 15:56:23 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"


// static int	ft_strncmp(const char *s1, const char *s2, size_t n)
// {
// 	size_t		i;

// 	i = 0;
// 	if (n == 0)
// 		return (0);
// 	while (s1[i] && s2[i] && i < n - 1 && s1[i] == s2[i])
// 		i++;
// 	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
// }

// static size_t	ft_strlen(const char *s)
// {
// 	size_t	i;

// 	if (!s)
// 		return (0);
// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

void	heredoc(t_cmd *test)
{
	int pipeheredoctest[2];
	char *line;
	char *delimiter;
	delimiter = test->redir->filename;
	
	if (pipe(pipeheredoctest) < 0)
	{
		perror("fail pipe");
		return ;
	}
	int pid = fork();
	if (pid == 0)
	{
		close(pipeheredoctest[0]);
		//dup2(pipeheredoctest[1], 1);
		while (1)
		{
			line = readline("testheredoc : ");
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			{
				if (delimiter[ft_strlen(delimiter)] == '\0')
				{
					free(line);
					break;
				}
			}
			write(pipeheredoctest[1], line, ft_strlen(line));
			write(pipeheredoctest[1], "\n", 1);
		}
		close(pipeheredoctest[1]);
		exit(0);
	}
	if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
		close(pipeheredoctest[1]);
		dup2(pipeheredoctest[0], 0);
		close(pipeheredoctest[0]);
	}
}
