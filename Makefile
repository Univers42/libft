# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
AR = ar rcs

# Library name
NAME = libft.a

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

# Source subdirectories
CTYPE_DIR = $(SRC_DIR)/ctype
STDIO_DIR = $(SRC_DIR)/stdio
STDLIB_DIR = $(SRC_DIR)/stdlib
STRING_DIR = $(SRC_DIR)/string
MEMORY_DIR = $(SRC_DIR)/memory
LISTS_DIR = $(SRC_DIR)/lists

# Source files by category
CTYPE_SRC = ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
            ft_isprint.c ft_isspace.c ft_tolower.c ft_toupper.c

STDIO_SRC = ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
            ft_putnbr_base.c ft_log10.c

STDLIB_SRC = ft_atoi.c ft_atoi_base.c ft_calloc.c ft_itoa.c ft_utoa.c ft_xtoa.c

STRING_SRC = ft_bzero.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c \
             ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c \
             ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
             ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c

MEMORY_SRC = ft_print_memory.c ft_print_hex.c ft_print_ascii.c

LISTS_SRC = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
            ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c

# Create full paths
CTYPE_SRCS = $(addprefix $(CTYPE_DIR)/, $(CTYPE_SRC))
STDIO_SRCS = $(addprefix $(STDIO_DIR)/, $(STDIO_SRC))
STDLIB_SRCS = $(addprefix $(STDLIB_DIR)/, $(STDLIB_SRC))
STRING_SRCS = $(addprefix $(STRING_DIR)/, $(STRING_SRC))
MEMORY_SRCS = $(addprefix $(MEMORY_DIR)/, $(MEMORY_SRC))
LISTS_SRCS = $(addprefix $(LISTS_DIR)/, $(LISTS_SRC))

# Combine all sources
SRCS = $(CTYPE_SRCS) $(STDIO_SRCS) $(STDLIB_SRCS) $(STRING_SRCS) $(MEMORY_SRCS)
BONUS_SRCS = $(LISTS_SRCS)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
BONUS_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(BONUS_SRCS))

# Header files
HEADERS = $(INC_DIR)/libft.h

# Progress bar and formatting
TOTAL_FILES = $(words $(SRCS))
CURRENT_FILE = 0
define progress-bar
	$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	@printf "\r\033[K"
	@printf "\033[1;34m[%3d%%]\033[0m " $(shell echo $$(($(CURRENT_FILE)*100/$(TOTAL_FILES))))
	@printf "\033[1;32mCompiling: \033[0m"
	@printf "\033[1;33m$<\033[0m"
endef

# Make sure the object directory exists
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(call progress-bar)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

# Create object directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/ctype
	@mkdir -p $(OBJ_DIR)/stdio
	@mkdir -p $(OBJ_DIR)/stdlib
	@mkdir -p $(OBJ_DIR)/string
	@mkdir -p $(OBJ_DIR)/memory
	@mkdir -p $(OBJ_DIR)/lists

# Rules
all: banner $(NAME)

banner:
	@echo "\033[1;36m"
	@echo "╔══════════════════════════════════════════════════════╗"
	@echo "║                                                      ║"
	@echo "║                 LIBFT COMPILATION                    ║"
	@echo "║                                                      ║"
	@echo "╚══════════════════════════════════════════════════════╝"
	@echo "\033[0m"
	@echo "Author: $(shell whoami)"
	@echo "Date: $(shell date '+%Y-%m-%d %H:%M:%S')"
	@echo

$(NAME): $(OBJS)
	@echo "\n\033[1;32mCreating library $(NAME)...\033[0m"
	@$(AR) $(NAME) $(OBJS)
	@echo "\033[1;32m✓ Library created successfully!\033[0m"

bonus: banner $(OBJS) $(BONUS_OBJS)
	@echo "\n\033[1;32mCreating library with bonus...\033[0m"
	@$(AR) $(NAME) $(OBJS) $(BONUS_OBJS)
	@echo "\033[1;32m✓ Library with bonus created successfully!\033[0m"

clean:
	@echo "\033[1;33mCleaning object files...\033[0m"
	@$(RM) -r $(OBJ_DIR)
	@echo "\033[1;32m✓ Object files cleaned!\033[0m"

fclean: clean
	@echo "\033[1;33mRemoving library...\033[0m"
	@$(RM) $(NAME)
	@echo "\033[1;32m✓ Library removed!\033[0m"

re: fclean all

rebonus: fclean bonus

so: $(OBJS) $(BONUS_OBJS)
	@echo "\033[1;33mCreating shared library...\033[0m"
	@$(CC) -nostartfiles -shared -o libft.so $(OBJS) $(BONUS_OBJS)
	@echo "\033[1;32m✓ Shared library created!\033[0m"

# Show help
help:
	@echo "\033[1;36m"
	@echo "╔══════════════════════════════════════════════════════╗"
	@echo "║                    LIBFT MAKEFILE                    ║"
	@echo "╚══════════════════════════════════════════════════════╝"
	@echo "\033[0m"
	@echo "\033[1;33mAvailable commands:\033[0m"
	@echo "  \033[1;32mmake\033[0m        - Compile the library"
	@echo "  \033[1;32mmake bonus\033[0m  - Compile with bonus functions (lists)"
	@echo "  \033[1;32mmake clean\033[0m  - Remove object files"
	@echo "  \033[1;32mmake fclean\033[0m - Remove object files and library"
	@echo "  \033[1;32mmake re\033[0m     - Recompile the library"
	@echo "  \033[1;32mmake so\033[0m     - Create a shared library"
	@echo "  \033[1;32mmake help\033[0m   - Show this help message"

.PHONY: all bonus clean fclean re rebonus so banner help