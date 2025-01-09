NAME = philosophers
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -pthread

SRCS = main.c \
	utils.c \
	parser.c \
	controlled_functions.c \
	init.c \
	dinner.c \
	mutex.c \
	syncro.c \
	write.c \
	monitor.c \

OBJS = $(SRCS:.c=.o)

.o : .c
	$(CC) $(CFLAGS) -c -o $@ $<.o : .c

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re