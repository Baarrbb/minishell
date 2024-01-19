/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:10:02 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 17:24:46 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <linux/limits.h>
# include <string.h>
# include <errno.h>
# include "libft.h"

typedef struct s_redir
{
	int				out;
	int				out_end;
	int				in;
	int				in_read;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**path;
	char			**cmd;
	int				builtin;
	char			*path_cmd;
	int				exit_val;
	int				*cmd_quote;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

# include "parsing.h"
# include "builtins.h"

# define ERROR_MSG "minishell: syntax error near unexpected token "

#endif