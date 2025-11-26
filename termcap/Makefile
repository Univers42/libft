NAME = libtermcap.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror -I.
AR = ar rcs
OBJDIR = obj

SRCS = $(wildcard *.c) $(wildcard rl/*.c)
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

# Default: build the test binary (will build the library first if needed)
all: test

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(OBJDIR)/%.o: %.c termcap.h
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -rf $(OBJDIR)

re: fclean all

# Build test binary; depends on library so it will be built if missing
test: $(NAME)
	mkdir -p bin
	$(CC) $(CFLAGS) tests/test_termcap.c -L. -ltermcap -o bin/test

shelltest: $(NAME)
	mkdir -p bin
	$(CC) $(CFLAGS) tests/test_shell.c -L. -ltermcap -o bin/test_shell

.PHONY: all clean fclean re test shelltest
