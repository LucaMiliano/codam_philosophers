NAME = philosophers

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
# CFLAGS = -Wall -Wextra -Werror -g

SRCS =	philosophers.c \
		init.c \
		monitor.c \
		mutexes.c \
		utils.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(OBJS) $(NAME)

re: fclean all

.PHONY: all clean fclean re
