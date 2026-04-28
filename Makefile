NAME = ft_ping

SRCS += ft_ping.c \
		srcs/ft_parser.c \
		srcs/ft_socket.c \
		srcs/ft_signal.c \
		srcs/helpers.c

OBJS = $(SRCS:%.c=%.o)

CC = gcc
FLAGS = -Wall -Wextra -Werror	

all: $(NAME)

$(NAME): $(OBJS)

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $<

clean:
	@rm -f *.o */*.o

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re