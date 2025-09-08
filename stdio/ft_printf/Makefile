# Makefile for mini_shell ft_printf test
NAME = test_printf
LIBFTPRINTF = libftprintf.a
SRC_DIR = src
INCLUDE_DIR = include
MEMORY_DIR = ../../memory
DEF_DIR = ../../render
CTYPE_DIR = ../../ctype
STRING_DIR = ../../strings
MATH_DIR = ../../math
DSA_DIR = ../../data_structures
ALGEBRA_DIR = ../../math/algebra
GEO_DIR = ../../math/geometry
PIXEL_DIR = ../../math/geometry/pixel
SRCS = $(shell find ../../ -name '*.c')
INC_DIRS = $(shell find ../../ -type f -name '*.h' -exec dirname {} \; | sort -u)
OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -Wextra -Werror $(addprefix -I, $(INC_DIRS))

all: $(LIBFTPRINTF) $(NAME)

$(LIBFTPRINTF): $(OBJS)
	ar rcs $@ $^

$(NAME): main.o $(LIBFTPRINTF)
	$(CC) $(CFLAGS) -o $@ main.o -L. -lftprintf

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f $(SRC_DIR)/*.o main.o

fclean: clean
	rm -f $(LIBFTPRINTF) $(NAME)

re: fclean all

print-includes:
	@echo "INCLUDE_DIR: $(INCLUDE_DIR)"
	@echo "CFLAGS: $(CFLAGS)"

.PHONY: all clean fclean re print-includes
