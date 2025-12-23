NAME = philo

CC = cc

# flags for debugging:
CFLAGS = -Wall -Wextra -Werror

SRCS =	main.c \
		philo_actions.c \
		threads.c \
		monitor.c \
		mutexes.c \
		utils.c \
		libft_functions.c \

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
