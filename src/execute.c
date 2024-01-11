/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:53:56 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/11 12:04:30 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <fcntl.h>
#include <assert.h>

/*void	execute_test(const t_cmd *pipes, char **envp)
{
	t_cmd	*commands;
	int		tmpinout[2];
	int		pipefd[2];
	int		fdin;
	int		fdout;
	pid_t	fils;

	tmpinout[0] = dup(0);
	tmpinout[1] = dup(1);
	commands = (t_cmd *)pipes;
	if (commands->redir != NULL)
	{
		if (commands->redir->in)
			fdin = open(commands->redir->filename, O_RDWR, 0000644);
		else if (commands->redir->in_read)
			fdin = open(commands->redir->filename, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
		else
			fdin = tmpinout[0];
	}
	else
		fdin = tmpinout[0];
	while (commands != NULL)
	{
		dup2(fdin, STDIN_FILENO);
		printf("fdin : %d\n", fdin);
		close (fdin);
		if (commands->next == NULL)
		{
			if (commands->redir != NULL)
			{
				if (commands->redir->out)
					fdout =open(commands->redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0000644);
				else if (commands->redir->out_end)
					fdout = open(commands->redir->filename, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
			}
			else
				fdout = dup(tmpinout[1]);
		}
		else
		{
			pipe(pipefd);
			fdout = pipefd[1];
			fdin = pipefd[0];
		}
		dup2(fdout, 1);
		close (fdout);
		fils = fork();
		if (fils == 0)	
			execve(commands->path_cmd, commands->cmd, envp);
		commands = commands->next;
	}
	printf("test");
	dup2(tmpinout[0], 0);
	dup2(tmpinout[1], 1);
	waitpid(0, NULL, 0);
}*/
/*
void	execute_test(const t_cmd *pipes, char **envp)
{
	pid_t	forks;
	pid_t	forkt;
	int		pipefd[2];
	t_cmd	*commands;
	int		base[2];

	commands = (t_cmd *)pipes;
	base[0] = dup(0);
	base[1] = dup(1);
	if (pipe(pipefd) < 0)
			printf("error");
	if (commands->next != NULL)
	{
		forks = fork();
		dup2(pipefd[1], STDOUT_FILENO);
		//dup2(av1, STDIN_FILENO);
		if (forks == 0)
			execve(commands->path_cmd, commands->cmd, envp);
	}
	else
	{
		forks = fork();
		if (forks == 0)
			execve(commands->path_cmd, commands->cmd, envp);
	}
	commands = commands->next;
	while (commands)
	{
		if (pipe(pipefd) < 0)
			printf("error");
		forks = fork();
		if (commands->next == NULL)
		{
			dup2(base[0], STDOUT_FILENO);
			dup2(pipefd[1], STDIN_FILENO);
			if (forks == 0)
				execve(commands->path_cmd, commands->cmd, envp);
		}
		else
		{
			dup2(pipefd[1], STDOUT_FILENO);
			dup2(pipefd[0], STDIN_FILENO);
			if (forks == 0)
				execve(commands->path_cmd, commands->cmd, envp);
		}
		commands = commands->next;
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(forks, NULL, 0);
	//waitpid(forkt, NULL, 0);
}
*/

void	redirections_pipe_in(t_cmd *redirec, int j, int *pipefds)
{
	if (redirec->next)
	{
		if(dup2(pipefds[j + 1], 1) < 0)
		{
			perror("dup2 error to do");
			exit(EXIT_FAILURE);
		}
	}
}

void	redirections_in(t_cmd *redirec, int fd)
{
	t_cmd	*newpointer;
	t_redir	*oldredir;

	newpointer = redirec;
	oldredir = newpointer->redir;
	while (newpointer->redir != NULL)
	{
		if (newpointer->redir->in)
		{
			fd = open(newpointer->redir->filename, O_RDONLY);
			dup2(fd, 0);
			//close(fd);
		}
		newpointer->redir = newpointer->redir->next;
	}
	redirec->redir = oldredir;
}

void	redirections_pipe_out(t_cmd *redirec, int j, int *pipefds)
{
	if (j != 0)
	{
		if (dup2(pipefds[j-2], 0) < 0)
		{
			perror("dup2 error to do");
			exit(EXIT_FAILURE);
		}
	}
}

void	redirections_out(t_cmd *redirec, int fd)
{
	t_cmd	*newpointer;
	t_redir	*oldredir;

	newpointer = redirec;
	oldredir = newpointer->redir;
	while (newpointer->redir != NULL)
	{
		if (newpointer->redir->out)
		{
			fd = open(newpointer->redir->filename, O_CREAT | O_RDWR | O_TRUNC, 0000644);
			dup2(fd, 1);
			//close(fd);
		}
		else if (newpointer->redir->out_end)
		{
			fd = open(newpointer->redir->filename, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
			dup2(fd, 1);
			//close(fd);
		}
		newpointer->redir = newpointer->redir->next;
	}
	redirec->redir = oldredir;
}

void	close_all_pipes(int numPipes, int *pipefds)
{
	int	i;

	i = 0;
	while (i < 2 * numPipes)
	{
		close(pipefds[i]);
		i++;
	}
}

int	count_struct(t_cmd *list)
{
	int	i;

	i = 0;
	if (list == NULL)
		return (0);
	while (list)
	{
		list = list->next;
		i++;
	}
	return (i);
}

void execute_test(t_cmd *pipes, char **envp) 
{
	t_cmd *command = (t_cmd *)pipes;
	int numPipes = count_struct(command);
	int status;
	int i;
	pid_t pid;
	int pipefds[2*numPipes];
	int fd = -1;
	int pipeindex;

	i = 0;
	//creation pipes create_pipes(*pipefds);
	while (numPipes > i)
	{
		if (pipe(pipefds + i*2) < 0) 
		{
			perror("couldn't pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	//forking
	pipeindex = 0;
	while (command) 
	{
		if (command->builtin)
			builtingo(command, envp);
		else
		{
			pid = fork();
			if (pid == 0) 
			{
				//not last command
				redirections_pipe_in(command, pipeindex, pipefds);
				//redirction out
				redirections_out(command, fd);
				//if not first command
				redirections_pipe_out(command, pipeindex, pipefds);
				//redirection_in
				redirections_in(command, fd);
				//close all pipes
				close_all_pipes(numPipes, pipefds);
				//gestion d'erreur
				error_managing(command);
				//execution
				if (execve(command->path_cmd, command->cmd, envp) < 0)
				{
					perror(command->path_cmd);
					exit(127);
				}
			}
			else if (pid < 0)
			{
				perror("dup2 error to do");
				exit(EXIT_FAILURE);
			}
		}
		command = command->next;
		pipeindex += 2;
	}
	//close pipe
	close_all_pipes(numPipes, pipefds);
	//wait for children & exitval
	i = 0;
	while (i < numPipes + 1)
	{
		pipes->exit_val = waitpid(pid, &status, 0);
		i++;
	}
}
