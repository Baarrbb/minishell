/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 22:27:19 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 17:23:39 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# define ERROR_MSG "minishell: syntax error near unexpected token "

/* PARSING_C */
t_cmd	*check_line(char *line, t_cmd **pipe, char **envp);

/* FILL_REDIR_C */
int		is_redir(t_cmd **cmd, char **args, int i);

/* FILL_CMD_C */
void	fill_cmd(t_cmd **pipe, char **args);

/* FILL_STRUCT_C */
t_cmd	*fill_struct(t_cmd **pipe, char **args, char **envp);

/* CHECK_SYNTAX_C */
int		check_syntax(char **args, int size);

/* PARSING_UTILS_C */
char	*strjoin(char *dst, char *s);
int		is_space(int c);
int		is_spe_char(int c);

/* COPY_ENV_C */
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