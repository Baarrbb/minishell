/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:27:19 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/23 15:44:06 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"
# include <unistd.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <assert.h>

# define BOLD "\x1b[1m"
# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define ERROR_MSG "bash: syntax error near unexpected token "
# define FILE_O_FLAGS (O_WRONLY | O_APPEND | O_CREAT)
# define FILE_PERM (S_IRUSR | S_IWUSR)
# define FILE_O_FLAGS_RDWR (O_CREAT | O_RDWR | O_TRUNC)
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
	int				*quote_cmd;
	int				builtin;
	char			*path_cmd;
	int				exit_val;
	int				background;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_exec
{
	t_cmd	*command;
	int		numpipes;
	int		index;
	pid_t	*pid;
	int		*pipefds;
	int		fd;
	int		pipeindex;
}	t_exec;

void	free_char_tab(char **tab);
void	free_list(t_cmd **list);
void	ft_lstadd_back(t_redir **lst, t_redir *new);
void	ft_lstadd_back_bis(t_cmd **lst, t_cmd *new);
char	*strjoin(char *dst, char *s);

/*  TO REMOVE */
void	print_struct(t_cmd *cmd);
void	print_redir(t_redir *redir);
void	print_linked_list(t_cmd *pipe);
/*test*/
int		execute_test(t_cmd *pipe, char ***envp);
int		check_commands(t_cmd *commands);
void	replace_variables(t_cmd *testons, char **envp);
int		handle_quoting(t_cmd *quoting, char **env, int sortie);
int		count_struct(t_cmd *list);
void	builtingo(t_cmd *cmd, char ***env);
void	error_exec(t_cmd *comands);
void	error_managing(t_cmd *command);
void	sigint_handler(int sig);
void	handle_sigint(int sig);
int		heredoc(t_cmd *test);

/* handle variables*/
char *get_ourenv(char* tofind, char **ourenv, char *sortie);
int	in_quotes(char test, int etat);
int		handle_var(t_cmd *cmd, char **our_envp, char *sortie);

/*redirections :*/
void	redirections_pipe_out(t_cmd *command, t_exec *data);
void	redirections_in(t_cmd *cmd);
void	redirections_pipe_in(t_exec *data);
int		redirections_out(t_cmd *cmd);
/*pipes opening and closing*/
void	close_all_pipes(int numPipes, int *pipefds);
int		numbers_pipe(t_cmd *list);
void	creation_pipes(t_exec *yipi);
/*signals handling*/
void	sig_ignore(void);
void	sig_default(void);
void	printtestsignals(int status);

#endif