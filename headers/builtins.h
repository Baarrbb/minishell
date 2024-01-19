/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:14:03 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 17:22:49 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"
# define ERROR_CD "minishell: cd:"

/* ECHO_C */
/*  CD_C  */
void	our_cd(t_cmd *cmd, char **env);
/* PWD_C */
/* EXPORT_C */
/* UNSET_C */
/* ENV_C */
void	our_env(char **env);
/* EXIT_C */

#endif