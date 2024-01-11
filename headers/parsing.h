/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:27:19 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/11 10:37:05 by ytouihar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

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
	int				*quote_cmd;
	int				builtin;
	char			*path_cmd;
	int				exit_val;
	int				background;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;


void	free_char_tab(char **tab);
void	free_list(t_cmd **list);
void	ft_lstadd_back(t_redir **lst, t_redir *new);
void	ft_lstadd_back_bis(t_cmd **lst, t_cmd *new);

/*  TO REMOVE */
void	print_struct(t_cmd *cmd);
void	print_redir(t_redir *redir);
void	print_linked_list(t_cmd *pipe);
/*test*/
void	execute_test(t_cmd *pipe, char **envp);
int		check_commands(t_cmd *commands);
void	is_a_variable(t_cmd *testons, char **envp);
int		handle_quoting(t_cmd *quoting);
int		count_struct(t_cmd *list);
void	builtingo(t_cmd *cmd, char **env);
void	error_exec(t_cmd *comands);
void	error_managing(t_cmd *command);
void	sigint_handler(int sig);
#endif