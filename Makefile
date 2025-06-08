# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 12:30:42 by dlesieur          #+#    #+#              #
#    Updated: 2025/06/07 01:02:48 by dlesieur         ###   ########.fr        #
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
RENDER_UTILS = render/ft_table_utils.c
ALL_SRCS = $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS) $(RENDER_UTILS)

# Object files with directory structure preserved
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))

# Header files
HEADERS = $(wildcard *.h)

# Enhanced colors and formatting
GREEN = \033[0;32m
BRIGHT_GREEN = \033[1;32m
RED = \033[0;31m
BRIGHT_RED = \033[1;31m
YELLOW = \033[0;33m
BRIGHT_YELLOW = \033[1;33m
BLUE = \033[0;34m
BRIGHT_BLUE = \033[1;34m
MAGENTA = \033[0;35m
BRIGHT_MAGENTA = \033[1;35m
CYAN = \033[0;36m
BRIGHT_CYAN = \033[1;36m
WHITE = \033[1;37m
RESET = \033[0m
BOLD = \033[1m
DIM = \033[2m
UNDERLINE = \033[4m

# Progress symbols
PROGRESS_CHARS = ▏▎▍▌▋▊▉█
SPINNER_CHARS = ⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏

# Progress counter using simpler approach
TOTAL_FILES = $(words $(ALL_SRCS))
COMPILED_COUNT = $(words $(wildcard $(OBJ_DIR)/*/*.o $(OBJ_DIR)/*/*/*.o $(OBJ_DIR)/*/*/*/*.o))

# Build start time
START_TIME = $(shell date +%s)

# Default rule: build the library
all: banner pre_build $(NAME) completion_banner
	@echo ""

banner:
	@echo "$(BRIGHT_CYAN)╔══════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "$(BRIGHT_CYAN)║                    🏗️  LIBFT BUILD SYSTEM 🏗️                    ║$(RESET)"
	@echo "$(BRIGHT_CYAN)╠══════════════════════════════════════════════════════════════╣$(RESET)"
	@echo "$(BRIGHT_CYAN)║$(RESET) $(WHITE)Library:$(RESET) $(BRIGHT_YELLOW)$(NAME)$(RESET)                                           $(BRIGHT_CYAN)║$(RESET)"
	@echo "$(BRIGHT_CYAN)║$(RESET) $(WHITE)Files:$(RESET)   $(BRIGHT_GREEN)$(TOTAL_FILES) source files$(RESET)                            $(BRIGHT_CYAN)║$(RESET)"
	@echo "$(BRIGHT_CYAN)║$(RESET) $(WHITE)Dirs:$(RESET)    $(BRIGHT_BLUE)$(words $(SRC_DIRS)) directories$(RESET)                                $(BRIGHT_CYAN)║$(RESET)"
	@echo "$(BRIGHT_CYAN)╚══════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""

pre_build:
	@echo "$(BRIGHT_BLUE)🔧 Setting up build environment...$(RESET)"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/stdio/ft_printf_42/src
	@mkdir -p $(OBJ_DIR)/stdio/gnl
	@mkdir -p $(foreach dir,$(SRC_DIRS),$(OBJ_DIR)/$(dir))
	@echo "$(GREEN)✅ Build directories created$(RESET)"
	@echo ""

# Rule to build the library
$(NAME): $(OBJS)
	@echo ""
	@echo "$(BRIGHT_MAGENTA)📦 Archiving library...$(RESET)"
	@printf "$(YELLOW)Creating $(NAME)... "
	@$(AR) $(NAME) $(OBJS)
	@echo "$(BRIGHT_GREEN)✓ Done!$(RESET)"

# Simplified compilation with reliable progress tracking
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@CURRENT_COUNT=$$(find $(OBJ_DIR) -name "*.o" | wc -l); \
	DISPLAY_COUNT=$$((CURRENT_COUNT)); \
	PERCENT=$$((CURRENT_COUNT * 100 / $(TOTAL_FILES))); \
	TERM_WIDTH=$$(tput cols 2>/dev/null || echo "80"); \
	BAR_WIDTH=$$((TERM_WIDTH - 50)); \
	if [ $$BAR_WIDTH -lt 20 ]; then BAR_WIDTH=20; fi; \
	if [ $$BAR_WIDTH -gt 40 ]; then BAR_WIDTH=40; fi; \
	FILLED=$$((CURRENT_COUNT * BAR_WIDTH / $(TOTAL_FILES))); \
	EMPTY=$$((BAR_WIDTH - FILLED)); \
	printf "\r\033[K$(BRIGHT_CYAN)Compiling$(RESET) $(DIM)[$$DISPLAY_COUNT/$(TOTAL_FILES)]$(RESET) "; \
	printf "$(BRIGHT_GREEN)"; \
	for i in $$(seq 1 $$FILLED); do printf "█"; done; \
	printf "$(RESET)$(DIM)"; \
	for i in $$(seq 1 $$EMPTY); do printf "░"; done; \
	printf "$(RESET) $(BRIGHT_YELLOW)%3d%%$(RESET) $(WHITE)%-15s$(RESET)" $$PERCENT "$$(basename $< .c)"; \
	if [ $$CURRENT_COUNT -eq $(TOTAL_FILES) ]; then \
		printf "\r\033[K$(BRIGHT_GREEN)Compiled$(RESET) $(DIM)[$$CURRENT_COUNT/$(TOTAL_FILES)]$(RESET) "; \
		printf "$(BRIGHT_GREEN)"; \
		for i in $$(seq 1 $$BAR_WIDTH); do printf "█"; done; \
		printf "$(RESET) $(BRIGHT_GREEN)100%% Complete!$(RESET)\n"; \
	fi

completion_banner:
	@$(eval END_TIME=$(shell date +%s))
	@$(eval BUILD_TIME=$(shell echo $$(($(END_TIME) - $(START_TIME)))))
	@echo ""
	@echo "$(BRIGHT_GREEN)╔══════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "$(BRIGHT_GREEN)║                   🎉 BUILD COMPLETED! 🎉                     ║$(RESET)"
	@echo "$(BRIGHT_GREEN)╠══════════════════════════════════════════════════════════════╣$(RESET)"
	@echo "$(BRIGHT_GREEN)║$(RESET) $(WHITE)Library:$(RESET)     $(BRIGHT_YELLOW)$(NAME)$(RESET) $(BRIGHT_GREEN)successfully created!$(RESET)      $(BRIGHT_GREEN)║$(RESET)"
	@echo "$(BRIGHT_GREEN)║$(RESET) $(WHITE)Files:$(RESET)       $(BRIGHT_CYAN)$(TOTAL_FILES) files compiled$(RESET)                      $(BRIGHT_GREEN)║$(RESET)"
	@echo "$(BRIGHT_GREEN)║$(RESET) $(WHITE)Build time:$(RESET)  $(BRIGHT_MAGENTA)$(BUILD_TIME) seconds$(RESET)                            $(BRIGHT_GREEN)║$(RESET)"
	@echo "$(BRIGHT_GREEN)║$(RESET) $(WHITE)Status:$(RESET)      $(BRIGHT_GREEN)✅ Ready for linking$(RESET)                  $(BRIGHT_GREEN)║$(RESET)"
	@echo "$(BRIGHT_GREEN)╚══════════════════════════════════════════════════════════════╝$(RESET)"

# Test program with enhanced feedback
test: $(NAME) main.c
	@echo "$(BRIGHT_BLUE)🧪 Building test program...$(RESET)"
	@$(CC) $(CFLAGS) main.c -L. -lft -o test_program
	@echo "$(BRIGHT_GREEN)✅ Test program compiled$(RESET)"
	@echo "$(BRIGHT_CYAN)🚀 Running tests...$(RESET)"
	@echo "$(YELLOW)═══════════════════════════════════════════════════════════════$(RESET)"
	@./test_program
	@echo "$(YELLOW)═══════════════════════════════════════════════════════════════$(RESET)"

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

# Clean test program
clean_test:
	@echo "$(RED)🗑️  Cleaning test artifacts...$(RESET)"
	@$(RM) test_program
	@echo "$(GREEN)✅ Test files cleaned$(RESET)"

# Rebuild with banner
re: fclean_banner all

fclean_banner:
	@echo "$(BRIGHT_MAGENTA)🔄 Rebuilding from scratch...$(RESET)"
	@$(MAKE) fclean --no-print-directory

# Debug mode with enhanced feedback
debug: CFLAGS += -g3 -fsanitize=address
debug: 
	@echo "$(BRIGHT_YELLOW)🐛 DEBUG MODE ENABLED$(RESET)"
	@echo "$(YELLOW)Flags: $(CFLAGS)$(RESET)"
	@$(MAKE) re --no-print-directory
	@echo "$(BRIGHT_YELLOW)🔍 Debug build completed with AddressSanitizer$(RESET)"

# Enhanced help with styled output
help:
	@echo "$(BRIGHT_CYAN)╔══════════════════════════════════════════════════════════════╗$(RESET)"
	@echo "$(BRIGHT_CYAN)║                    📚 LIBFT HELP MENU 📚                     ║$(RESET)"
	@echo "$(BRIGHT_CYAN)╚══════════════════════════════════════════════════════════════╝$(RESET)"
	@echo ""
	@echo "$(BRIGHT_WHITE)🔨 Build Targets:$(RESET)"
	@echo "  $(BRIGHT_GREEN)all$(RESET)        - Build the library with progress display"
	@echo "  $(BRIGHT_BLUE)re$(RESET)         - Clean rebuild from scratch"
	@echo "  $(BRIGHT_YELLOW)debug$(RESET)      - Build with debug flags and sanitizers"
	@echo ""
	@echo "$(BRIGHT_WHITE)🧪 Testing:$(RESET)"
	@echo "  $(BRIGHT_CYAN)test$(RESET)       - Build and run test program"
	@echo ""
	@echo "$(BRIGHT_WHITE)🧹 Cleanup:$(RESET)"
	@echo "  $(BRIGHT_RED)clean$(RESET)      - Remove object files"
	@echo "  $(BRIGHT_RED)fclean$(RESET)     - Remove all generated files"
	@echo "  $(BRIGHT_RED)clean_test$(RESET) - Remove test artifacts"
	@echo ""
	@echo "$(BRIGHT_WHITE)ℹ️  Information:$(RESET)"
	@echo "  $(BRIGHT_MAGENTA)help$(RESET)       - Show this help menu"
	@echo ""

# Show build statistics
stats:
	@echo "$(BRIGHT_BLUE)📊 Library Statistics:$(RESET)"
	@echo "$(YELLOW)═══════════════════════════════════════════════════════════════$(RESET)"
	@echo "  $(WHITE)Total source files:$(RESET) $(BRIGHT_GREEN)$(TOTAL_FILES)$(RESET)"
	@echo "  $(WHITE)Source directories:$(RESET) $(BRIGHT_BLUE)$(words $(SRC_DIRS))$(RESET)"
	@if [ -f "$(NAME)" ]; then \
		echo "  $(WHITE)Library size:$(RESET) $(BRIGHT_CYAN)$$(ls -lh $(NAME) | awk '{print $$5}')$(RESET)"; \
		echo "  $(WHITE)Library status:$(RESET) $(BRIGHT_GREEN)✅ Built$(RESET)"; \
	else \
		echo "  $(WHITE)Library status:$(RESET) $(BRIGHT_RED)❌ Not built$(RESET)"; \
	fi
	@echo "$(YELLOW)═══════════════════════════════════════════════════════════════$(RESET)"

# Declare phony targets
.PHONY: all clean fclean re debug pre_build help test clean_test banner completion_banner fclean_banner stats