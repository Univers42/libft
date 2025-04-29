NAME = debug_tester

CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Source files - include debug_tester.c and all debug implementation files
SRC = debug_tester.c \
	  ../../debug/ft_print_ascii.c \
	  ../../debug/ft_print_hex.c \
	  ../../debug/ft_print_memory.c

# Include directories
INCLUDES = -I../.. -I../../debug

# We might need these functions from libft
ADDITIONAL_SRC = ../../stdio/ft_putchar_fd.c \
                ../../stdio/ft_putstr_fd.c \
                ../../stdio/ft_putnbr_base.c \
				../utils/banner.c

all: $(NAME)

$(NAME):
	$(CC) $(CFLAGS) $(SRC) $(ADDITIONAL_SRC) -o $(NAME) $(INCLUDES)

run: $(NAME)
	./$(NAME)

clean:
	@echo "Nothing to clean"

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re run
