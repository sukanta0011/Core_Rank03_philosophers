SRC_DIR = srcs
HDR_DIR = includes
FLAGS = -Wall -Wextra -Werror -g
CC = gcc
ARGS = 3 800 200 200 2

# MY_SRCS = execute_cmds.c\
# 			utils1.c\
# 			main.c

# SRCS = $(addprefix $(SRC_DIR)/, $(MY_SRCS))
SRCS = $(wildcard srcs/*.c)
OBJS = $(SRCS:.c=.o)
NAME = philo_bonus

all: $(NAME)

$(NAME): $(OBJS)
# 	ar rcs $(NAME) $(OBJS)
	$(CC) $(FLAGS) -I$(HDR_DIR) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -I$(HDR_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

output: all
	$(addprefix ./, $(NAME)) $(ARGS)

leak: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(addprefix ./, $(NAME)) $(ARGS)

norm:
	norminette -R CheckDefine

.PHONY: all clean fclean out norm leak