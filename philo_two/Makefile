# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: khermann <khermann@21-school.ru>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/02/22 13:40:07 by khermann          #+#    #+#              #
#    Updated: 2021/05/17 22:44:46 by khermann         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    =   philo_two

HEADER 	=   ./philo_two.h

SRCS    =	main.c \
			parser.c \
			philosophers.c \
			waiter.c \
			ph_utils.c



OBJS		=	$(SRCS:.c=.o)

CC      	=   gcc

CFLAGS  	=  -Wall -Wextra #-fsanitize=address  #-Werror

.PHONY:		all clean fclean re bonus

all:			$(NAME)

$(NAME):			$(HEADER) $(OBJS)
					$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
					$(RM) $(OBJS) $(OBJSBONUS)

fclean:			clean
					$(RM) $(NAME)

re:			fclean  all
