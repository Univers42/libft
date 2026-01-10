LIB = libpipe.a
SRC = src/pipe_create.c \
      src/pipe_close_pair.c \
      src/pipe_close_end.c \
      src/write_all.c \
      src/read_all.c \
      src/read_line_arena.c \
      src/set_nonblock.c \
      src/set_cloexec.c \
      src/ft_memory.c
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I include

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(LIB)

re: fclean all

.PHONY: all clean fclean re