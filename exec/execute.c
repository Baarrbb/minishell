/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 11:53:56 by ytouihar          #+#    #+#             */
/*   Updated: 2024/01/23 15:45:34 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_exec	*fill_struct_exec(t_cmd *command)
{
	t_exec	*yipi;

	yipi = malloc(1 * sizeof(t_exec));
	if (yipi == NULL)
		return (0);
	yipi->index = 0;
	yipi->numpipes = numbers_pipe(command);
	yipi->pid = malloc((yipi->numpipes + 1) * sizeof(pid_t));
	if (yipi->pid == NULL)
	{
		free(yipi);
		return (0);
	}
	yipi->pid[yipi->numpipes] = '\0';
	yipi->pipefds = malloc(((yipi->numpipes * 2)) * sizeof(int));
	if (yipi->pipefds == NULL)
	{
		free(yipi->pid);
		free(yipi);
		return (0);
	}
	yipi->pipeindex = 0;
	creation_pipes(yipi);
	return (yipi);
}

static void	free_struct_exec(t_exec *yipi)
{
	free(yipi->pipefds);
	free(yipi->pid);
	free(yipi);
}

static void	exec(t_cmd *command, t_exec *yipi, char **envp)
{
	yipi->pid[yipi->index] = fork();
	if (yipi->pid[yipi->index] == 0)
	{
		sig_default();
		redirections_pipe_out(command, yipi);
		redirections_out(command);
		redirections_pipe_in(yipi);
		redirections_in(command);
		close_all_pipes(yipi->numpipes, yipi->pipefds);
		error_managing(command);
		if (execve(command->path_cmd, command->cmd, envp) < 0)
		{
			perror(command->path_cmd);
			exit(127);
		}
	}
	else if (yipi->pid[yipi->index] < 0)
	{
		perror("dup2 error to do");
		exit(EXIT_FAILURE);
	}
}

static int	handle_waitpid(t_cmd *pipe, t_exec *yipi)
{
	int	status;
	int	wait_result;

	status = 0;
	wait_result = 0;
	yipi->index = 0;
	while (yipi->index < yipi->numpipes)
	{
		wait_result = waitpid(yipi->pid[yipi->index], &status, 0);
    	if (wait_result == -1)
		{
			perror("waitpid error");
			break ;
		}
		pipe->exit_val = wait_result;
		yipi->index++;
	}
	return (status);
}

int	execute_test(t_cmd *pipe, char ***envp)
{
	t_exec	*yipi;
	t_cmd	*command;
	int		status;

	command = (t_cmd *)pipe;
	status = 0;
	yipi = fill_struct_exec(command);
	if (yipi == NULL)
		return (-1);
	while (command)
	{
		sig_ignore();
		if (command->builtin)
			builtingo(command, envp);
		else
			exec(command, yipi, *envp);
		command = command->next;
		yipi->pipeindex += 2;
		yipi->index++;
	}
	close_all_pipes(yipi->numpipes, yipi->pipefds);
	status = handle_waitpid(pipe, yipi);
	printtestsignals(status);
	free_struct_exec(yipi);
	return (WEXITSTATUS(status));
}
