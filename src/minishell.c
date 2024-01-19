/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 00:07:33 by bsuc              #+#    #+#             */
/*   Updated: 2024/01/19 19:43:29 by bsuc             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_cmd	*pipe;
	char	**cpy_env;

	(void)ac;
	(void)av;
	pipe = 0;
	cpy_env = copy_env(envp);
	while (1)
	{
		line = readline("Minishell $ ");
		rl_on_new_line();
		if (!line)
			return (0);
		if (line[0] != ' ' && line[0] != 0)
			add_history(line);
		check_line(line, &pipe, cpy_env);
		print_linked_list(pipe);
		free_list(&pipe);
	}
	free_char_tab(cpy_env);
	return (0);
}
