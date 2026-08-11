NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = codexion.c monitor.c\
       init_rules.c \
       init_table.c \
       utils.c dongles_utils.c heap.c heap_utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
