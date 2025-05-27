# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 12:30:42 by dlesieur          #+#    #+#              #
#    Updated: 2025/05/27 18:48:31 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Library name
NAME = libft.a

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

# Directories
SRC_DIRS = ctype debug lists math memory render stdio stdlib strings
OBJ_DIR = obj

# Source files from all directories including ft_printf and gnl
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
PRINTF_SRCS = $(shell find stdio/ft_printf_42/src -name "*.c" 2>/dev/null)
GNL_SRCS = $(wildcard stdio/gnl/*.c)
ALL_SRCS = $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS)

# Object files with directory structure preserved
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))

# Header files
HEADERS = $(wildcard *.h)

# Colors and formatting
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[0;36m
RESET = \033[0m
BOLD = \033[1m

# Progress counter
TOTAL_FILES = $(words $(ALL_SRCS))
COUNTER = 0

# Default rule: build the library
all: pre_build $(NAME)
	@printf "$(GREEN)$(BOLD)[DONE] Library $(NAME) successfully created!$(RESET)\n"

pre_build:
	@printf "$(CYAN)$(BOLD)[BUILD] Building libft...$(RESET)\n"
	@mkdir -p $(OBJ_DIR)

# Rule to build the library
$(NAME): $(OBJS)
	@printf "\n$(MAGENTA)[ARCHIVE] Creating $(NAME)...$(RESET)\n"
	@$(AR) $(NAME) $(OBJS)

# Rule to compile .c files into .o files with directory structure and progress bar
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
	@$(CC) $(CFLAGS) -I. -Istdio/ft_printf_42/include -c $< -o $@
	@printf "\r$(YELLOW)Compiling: [%-30s] %d%%$(RESET)" "$(shell printf '%*s' $$(($(COUNTER) * 30 / $(TOTAL_FILES))) | tr ' ' '▓')" "$$(($(COUNTER) * 100 / $(TOTAL_FILES)))"

# Test program
test: $(NAME) main.c
	@printf "$(CYAN)[TEST] Compiling test program...$(RESET)\n"
	@$(CC) $(CFLAGS) main.c -L. -lft -o test_program
	@printf "$(GREEN)[TEST] Running test program:$(RESET)\n"
	@./test_program

# Clean object files
clean:
	@printf "$(RED)[CLEAN] Removing object files...$(RESET)\n"
	@$(RM) -r $(OBJ_DIR)

# Clean object files and the library
fclean: clean
	@printf "$(RED)[CLEAN] Removing library $(NAME)...$(RESET)\n"
	@$(RM) $(NAME)

# Clean test program
clean_test:
	@$(RM) test_program

# Rebuild the library
re: fclean all

# Debug mode with sanitizers
debug: CFLAGS += -g3 -fsanitize=address
debug: re
	@printf "$(YELLOW)[DEBUG] Debug mode enabled with AddressSanitizer$(RESET)\n"

# Help command to show available targets
help:
	@printf "$(BOLD)Available commands:$(RESET)\n"
	@printf "  $(GREEN)make$(RESET)        - Build the library\n"
	@printf "  $(GREEN)make clean$(RESET)  - Remove object files\n"
	@printf "  $(GREEN)make fclean$(RESET) - Remove object files and library\n"
	@printf "  $(GREEN)make re$(RESET)     - Rebuild from scratch\n"
	@printf "  $(GREEN)make debug$(RESET)  - Build with debug symbols and sanitizers\n"

# Declare phony targets
.PHONY: all clean fclean re debug pre_build help test clean_test