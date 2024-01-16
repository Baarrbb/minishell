/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:53:56 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/16 14:54:53 by ytouihar         ###   ########.fr       */
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

void	redirections_in(t_cmd *cmd)
{
	int		fd;
	t_redir	*oldredir;

	oldredir = cmd->redir;
	fd = 0;
	if (cmd->redir)
		fd = -1;
	while (cmd->redir != NULL)
	{
		if (fd > 2)
			close(fd);
		if (cmd->redir->in)
			fd = open(cmd->redir->filename, O_RDONLY);
		else if (cmd->redir->in_read)
			fd = heredoc(cmd);
		cmd->redir = cmd->redir->next;
	}
	dup2(fd, 0);
	if (fd > 2)
		close(fd);
	cmd->redir = oldredir;
}

void	redirections_pipe_out(int j, int *pipefds)
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

void	redirections_out(t_cmd *cmd)
{
	t_redir	*oldredir;
	int		fd;

	fd = 1;
	if (cmd->redir)
		fd = -1;
	oldredir = cmd->redir;
	while (cmd->redir != NULL)
	{
		if (fd > 2)
			close(fd);
		if (cmd->redir->out)
			fd = open(cmd->redir->filename, FILE_O_FLAGS_RDWR, FILE_PERM);
		else if (cmd->redir->out_end)
			fd = open(cmd->redir->filename, FILE_O_FLAGS, FILE_PERM);
		cmd->redir = cmd->redir->next;
	}
	dup2(fd, 1);
	if (fd > 2)
		close(fd);
	cmd->redir = oldredir;
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

//void	free_execute(int *inttofree, pid_t *pidtofree)

int execute_test(t_cmd *pipes, char **envp) 
{
	t_cmd *command;
	int numPipes;
	int status;
	int index;
	pid_t *pid;
	int *pipefds;
	int fd;
	int pipeindex;

	// initialisation :
	command = (t_cmd *)pipes;
	numPipes = count_struct(command);
	index = 0;
	fd = 0;
	pid = malloc((numPipes + 1) * sizeof(pid_t));
	pid[numPipes] = '\0';
	pipefds = malloc(((numPipes * 2) + 1) * sizeof(int));
	pipefds[2 * numPipes] = '\0';
	pipeindex = 0;
	
	//creation pipes create_pipes(*pipefds);
	while (numPipes > index)
	{
		if (pipe(pipefds + index * 2) < 0) 
		{
			perror("couldn't pipe");
			exit(EXIT_FAILURE);
		}
		index++;
	}
	index = 0;
	while (command) 
	{
		signal(SIGINT, SIG_IGN);	
		signal(SIGQUIT, SIG_IGN);
		if (command->builtin)
			builtingo(command, envp);
		else
		{
			pid[index] = fork();
			if (pid[index] == 0) 
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				redirections_pipe_in(command, pipeindex, pipefds);
				redirections_out(command);
				redirections_pipe_out(pipeindex, pipefds);
				redirections_in(command);
				close_all_pipes(numPipes, pipefds);
				error_managing(command);
				if (execve(command->path_cmd, command->cmd, envp) < 0)
				{
					perror(command->path_cmd);
					exit(127);
				}
			}
			else if (pid[index] < 0)
			{
				perror("dup2 error to do");
				exit(EXIT_FAILURE);
			}
		}
		command = command->next;
		pipeindex += 2;
		index++;
	}
	close_all_pipes(numPipes, pipefds);
	free(pipefds);
	//wait for children & exitval
	index = 0;
	while (index < numPipes)
	{
		pipes->exit_val = waitpid(pid[index], &status, 0);
		index++;
	}
	free(pid);
	if (WIFSIGNALED(status) == -6) 
	{
		//printf("Le processus enfant a été terminé par le signal %d\n", WTERMSIG(status));
		//proly utilisable pour gerer lemessage d erreur
	}
	return (WEXITSTATUS(status));
}
