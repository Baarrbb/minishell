/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:27:19 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/13 19:19:18 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <errno.h>

#include <asm-generic/errno-base.h> // pour les erreurs chdri  normalement dans unistd.h mais chez moi trouve pas 

# define BOLD "\x1b[1m"
# define RED "\x1b[31m"
# define RESET "\x1b[0m"
# define ERROR_MSG "bash: syntax error near unexpected token "
# define ERROR_CD "bash: cd:"

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

void	free_char_tab(char **tab);
void	free_list(t_cmd **list);
void	ft_lstadd_back(t_redir **lst, t_redir *new);
void	ft_lstadd_back_bis(t_cmd **lst, t_cmd *new);
void	free_struct(t_cmd *cmd);
int		return_free(char *tofree, int ret);
char	*get_ourenv(char* tofind, char **ourenv);


/*  TO REMOVE */
void	print_struct(t_cmd *cmd);
void	print_redir(t_redir *redir);
void	print_linked_list(t_cmd *pipe);

#endif