# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

CC		= 	cc
CFLAGS	= 	-Wall -Wextra -Werror -I../include
LDFLAGS	= 	-MMD -MP
NAME	= 	ctype.a
SRCS	=	ft_hex_digit.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isblank.c ft_isdigit.c ft_isgraph.c \
			ft_islower.c ft_isprint.c ft_isspace.c ft_isupper.c ft_isxdigit.c ft_tolower.c ft_toupper.c

OBJ_DIR = obj
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
TESTOBJ = $(OBJ_DIR)/test.o
TESTBIN = test_ctype

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Path to the test framework static library (adjust if needed)
TEST_FRAMEWORK_DIR = ../testing
TEST_FRAMEWORK_LIB = $(TEST_FRAMEWORK_DIR)/libtest_framework.a
LIBFT = ../libft.a

# Build test binary (links all ctype code + test.c + test framework lib + libft.a)
$(TESTBIN): $(OBJS) $(TESTOBJ) $(TEST_FRAMEWORK_LIB) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@

test: $(TESTBIN)
	@echo "Run ./$(TESTBIN) to execute tests."

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(TESTBIN)

re: fclean all

.PHONY: all clean fclean re test