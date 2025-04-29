NAME = get_next_line.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = get_next_line.c get_next_line_utils.c \
	../../strings/ft_strlen.c \
	../../strings/ft_strclen.c \
	../../strings/ft_strndup.c \
	../../strings/ft_strjoin.c \
	../../memory/ft_memmove.c

OBJS = $(SRCS:.c=.o)

INCLUDES = -I.
LIBS =

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

main: $(NAME) main.o
	$(CC) $(CFLAGS) $(INCLUDES) main.o $(NAME) $(LIBS) -o main

main.o: main.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) main.o

fclean: clean
	rm -f $(NAME) main

re: fclean all

.PHONY: all clean fclean re
