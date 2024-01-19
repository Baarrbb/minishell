# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/29 23:25:19 by bsuc              #+#    #+#              #
#    Updated: 2024/01/19 17:32:37 by bsuc             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = src/minishell.c \
	src/parsing/check_syntax.c src/parsing/fill_cmd.c \
	src/parsing/fill_redir.c src/parsing/fill_struct.c \
	src/parsing/manage_list.c src/parsing/parsing_utils.c \
	src/parsing/parsing.c \
	src/copy_envp.c src/sanitize.c src/print.c
# src/builtins/cd.c src/builtins/env.c src/builtins/export.c 
OBJ = $(SRC:.c=.o)
NAME = minishell
RM = rm -f
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
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
	valgrind --suppressions=readline.supp --track-fds=yes --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell
