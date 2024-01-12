# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsuc <bsuc@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/15 17:04:31 by bsuc              #+#    #+#              #
#    Updated: 2024/01/12 22:25:49 by bsuc             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = ft_split.c ft_memset.c ft_strchr.c ft_substr.c ft_strlen.c \
	ft_bzero.c ft_calloc.c ft_strncmp.c ft_strnstr.c ft_strtrim.c \
	ft_isprint.c ft_strdup.c ft_itoa.c ft_atoi.c
OBJS = ${SRCS:.c=.o}
NAME = libft.a
RM = rm -f
CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -I../headers

all : ${NAME}

${NAME} : ${OBJS}
	ar rc ${NAME} ${OBJS}

%.o : %.c
	${CC} ${CFLAGS} $(INC) -c $< -o $@

clean :
	${RM} ${OBJS} ${OBJS_BONUS}

fclean : clean
	${RM} ${NAME}

re : fclean all

bonus : ${OBJS} ${OBJS_BONUS}
	ar rc ${NAME} ${OBJS} ${OBJS_BONUS}
