/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:27:19 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 00:46:11 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

#include "minishell.h"
#include <asm-generic/errno-base.h> // pour les erreurs chdri  normalement dans unistd.h mais chez moi trouve pas 

# define ERROR_MSG "minishell: syntax error near unexpected token "


t_cmd	*check_line(char *line, t_cmd **pipe, char **envp);

/* CHECK_SYNTAX_C */
int	check_syntax(char **args, int size);

/* PARSING_UTILS_C */
char	*strjoin(char *dst, char *s);
char	**copy_env(char **envp);

/* SANITIZE_C */
void	free_char_tab(char **tab);
void	free_list(t_cmd **list);

/*  MANAGE_LIST_C  */
void	ft_lstadd_back(t_redir **lst, t_redir *new);
void	ft_lstadd_back_bis(t_cmd **lst, t_cmd *new);

/*  PRINT_C */
void	print_struct(t_cmd *cmd);
void	print_redir(t_redir *redir);
void	print_linked_list(t_cmd *pipe);

#endif