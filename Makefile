# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/29 23:25:19 by bsuc              #+#    #+#              #
#    Updated: 2024/01/16 18:00:14 by ytouihar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = src/parsing.c src/sanitize.c  src/manage_list.c exec/execute.c exec/checks_comms.c \
	exec/builtins.c exec/errors.c exec/signals.c exec/heredoc.c exec/quote_removing.c \
	exec/pipes.c exec/redirections.c exec/handle_var_utils.c exec/handle_var.c
OBJ = $(SRC:.c=.o)
NAME = minishell
RM = rm -f
CC = cc
CFLAGS = -Wall -Wextra -g #-Werror
LIBFT = libft/libft.a
INC = -I./headers

all : $(NAME)

$(NAME) : start $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

start :
	make -C libft all

clean :
	make -C libft clean
	$(RM) $(OBJ)

fclean : clean
	make -C libft fclean
	$(RM) $(NAME)

re : fclean all

val : all
	valgrind --leak-check=full --track-origins=yes ./minishell