/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:27:19 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/04 17:18:01 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"
#include <unistd.h>

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
	char			**env_ms;
	char			**path;
	char			**cmd;
	int				builtin;
	char			*path_cmd;
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

#endif