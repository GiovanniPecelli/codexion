NAME = codexion
CC = cc


# ============== Compiling with -pthread ==================
# -pthread link a library and tells the compiler to define
# certain macros used behind the scenes.
# This ensures that standard C functions used in the
# program code behave in a thread-safe manner
# =========================================================
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = codexion.c monitor.c\
       init_rules.c \
       init_table.c dongles_utils.c\
       utils.c dongles.c heap.c heap_utils.c

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
