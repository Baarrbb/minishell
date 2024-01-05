# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ytouihar <ytouihar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/29 23:25:19 by bsuc              #+#    #+#              #
#    Updated: 2024/01/04 19:16:37 by ytouihar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = src/parsing.c src/sanitize.c  src/manage_list.c src/execute.c src/checks_comms.c
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