NAME = ctype_tester

SRC = ../../ctype/ft_isalpha.c ../../ctype/ft_isdigit.c ../../ctype/ft_isalnum.c \
      ../../ctype/ft_isascii.c ../../ctype/ft_isprint.c ../../ctype/ft_isspace.c \
      ../../ctype/ft_isupper.c ../../ctype/ft_islower.c ../../ctype/ft_toupper.c \
      ../../ctype/ft_tolower.c ../../ctype/ft_isblank.c ../../ctype/ft_isxdigit.c \
      ../../ctype/ft_isgraph.c \
      ft_ctype_tester.c ../utils/banner.c

OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I../../ctype

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
