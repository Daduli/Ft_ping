NAME = ft_ping

SRCS += ft_ping.c \
		srcs/ft_parser.c \
		srcs/ft_socket.c \
		srcs/ft_signal.c \
		srcs/ft_packet.c \
		srcs/helpers.c 

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror

$(NAME) :	$(OBJS)
	gcc $(CFLAGS) $(OBJS) -lm -o $(NAME)

all :	$(NAME)

clean:
	@rm -f *.o */*.o

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re