# Makefile for libft library

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Library name
NAME = libft.a

# Directories
SRC_DIRS = ctype lists memory stdio stdlib string
OBJ_DIR = obj

# Source files from all directories
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Object files with directory structure preserved
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

# Header files
HEADERS = $(wildcard *.h)

# Default rule: build the library
all: $(NAME)

# Rule to build the library
$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

# Rule to compile .c files into .o files with directory structure
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -rf $(OBJ_DIR)

# Clean object files and the library
fclean: clean
	rm -f $(NAME)

# Rebuild the library
re: fclean all

# Declare phony targets
.PHONY: all clean fclean re