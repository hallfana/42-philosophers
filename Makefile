NAME = philosophers
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -pthread

SRCS = main.c \
OBJS = $(SRCS:.c=.o)
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re