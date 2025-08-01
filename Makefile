# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 12:30:42 by dlesieur          #+#    #+#              #
#    Updated: 2025/08/01 10:45:16 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Static MAKEFILE includes
include build/colors.mk
include build/common.mk

define build_lib
	$(MAKE) -C $(1) $(2)
endef

define trans_remove_c
	find . -type f -name "main.c" -exec sh -c 'mv "$$1" "$${1%.c}.bak"' _ {} \;
endef

define trans_add_c
	find . -type f -name "main.bak" -exec sh -c 'mv "$$1" "$${1%.bak}.c"' _ {} \;
endef

# MAIN ?=

# Library name
NAME = libft.a
MINILIBX_DIR = minilibx-linux
MINILIBX_LIB = $(MINILIBX_DIR)/libmlx.a
# Directories
DATA_STRUCTURES = 	data_structures/queue data_structures/circular_linked_list data_structures/doubly_linked_list\
					data_structures/lists data_structures/vector classes/render/map classes/render/window classes/render/inputHandler classes/render/point classes/render/line

# classes classes/render classes/render/window classes/render/inputHandler classes/render/point/ classes/render/camera
SRC_DIRS = ctype debug $(DATA_STRUCTURES) math memory render stdio stdlib strings sort classes/render/inputHandler


# Source files from all directories including ft_printf and gnl
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
PRINTF_SRCS = $(shell find stdio/ft_printf_42/src -name "*.c" 2>/dev/null)
GNL_SRCS = $(wildcard stdio/gnl/*.c)
ALL_SRCS = $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS)

# Object files with directory structure preserved
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))

# Header files
HEADERS = $(wildcard *.h)

# Find all unique directories containing .h files in libft, always include root dir
HEADER_DIRS := $(shell find . -type f -name '*.h' -exec dirname {} \; | sort -u)
HEADER_DIRS += .
INCLUDE_FLAGS := $(foreach dir,$(HEADER_DIRS),-I$(dir))

# Add INCLUDE_FLAGS to CFLAGS
CFLAGS += $(INCLUDE_FLAGS) -g3

# Default rule: build the library
all: $(MINILIBX_LIB) $(NAME)

# Build MiniLibX
$(MINILIBX_LIB):
	@echo "$(BRIGHT_CYAN)🔧 Building MiniLibX...$(RESET)"
	@$(MAKE) -C $(MINILIBX_DIR)

# Rule to build the library
$(NAME): $(OBJS)
	@echo "$(BRIGHT_MAGENTA)📦 Archiving library...$(RESET)"
	@$(AR) $(NAME) $(OBJS)
	@echo "$(BRIGHT_GREEN)✓ $(NAME) created successfully!$(RESET)"

# Object compilation rule - simplified
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "$(CYAN)Compiling$(RESET) %-40s $(GREEN)✓$(RESET)\n" "$(notdir $<)"
	@$(CC) $(CFLAGS) -c $< -o $@

build :
	@call(build_lib($(MINILIBX), all));
	
# Enhanced clean with visual feedback
clean:
	@echo "$(BRIGHT_RED)🧹 Cleaning object files...$(RESET)"
	@if [ -d "$(OBJ_DIR)" ]; then \
		echo "$(YELLOW)Removing $(OBJ_DIR)/ directory...$(RESET)"; \
		$(RM) -r $(OBJ_DIR); \
		echo "$(GREEN)✅ Object files cleaned$(RESET)"; \
	else \
		echo "$(DIM)No object files to clean$(RESET)"; \
	fi
	@echo "$(BRIGHT_RED)🧹 Cleaning MiniLibX...$(RESET)"
	@$(MAKE) -C $(MINILIBX_DIR) clean

# Enhanced fclean with visual feedback
fclean: clean
	@echo "$(BRIGHT_RED)🔥 Deep cleaning...$(RESET)"
	@if [ -f "$(NAME)" ]; then \
		echo "$(YELLOW)Removing library $(NAME)...$(RESET)"; \
		$(RM) $(NAME); \
		echo "$(GREEN)✅ Library removed$(RESET)"; \
	else \
		echo "$(DIM)No library to remove$(RESET)"; \
	fi
	@echo "$(BRIGHT_RED)🔥 Deep cleaning MiniLibX...$(RESET)"
	@$(MAKE) -C $(MINILIBX_DIR) clean

# Rebuild
re: fclean all

# Debug mode with enhanced feedback
debug: CFLAGS += -g3 -fsanitize=address
debug: 
	@echo "$(BRIGHT_YELLOW)🐛 DEBUG MODE ENABLED$(RESET)"
	@echo "$(YELLOW)Flags: $(CFLAGS)$(RESET)"
	@$(MAKE) re --no-print-directory
	@echo "$(BRIGHT_YELLOW)🔍 Debug build completed with AddressSanitizer$(RESET)"

# Build a test executable from a main.c in a subdirectory
# Usage: make test TEST=window
TEST ?=

ifeq ($(TEST),)
TEST_MAIN :=
else
TEST_MAIN := $(shell find . -type f -path "*/$(TEST)/main.c" | head -n 1)
endif

TEST_EXE := $(TEST)

.PHONY: test $(TEST_EXE)
mode_42:
	@$(trans_remove_c)
mode_lab:
	@$(trans_add_c)

test: $(TEST_EXE)

$(TEST_EXE): $(MINILIBX_LIB) $(NAME)
ifneq ($(strip $(TEST_MAIN)),)
	$(CC) $(CFLAGS) -o $@ $(TEST_MAIN) $(NAME) $(MINILIBX_LIB) -lX11 -lXext -lm
else
	@echo "No main.c found for test '$(TEST)'"
	@exit 1
endif