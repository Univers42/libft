# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 12:30:42 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/22 19:04:46 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================================================================ #
#									INCLUDES								   #
# ============================================================================ #
include build/colors.mk
include build/common.mk

# ============================================================================ #
#									INCLUDES								   #
# ============================================================================ #
define build_lib
	$(MAKE) -C $(1) $(2)
endef

define trans_remove_c
	find . -type f -name "main.c" -exec sh -c 'mv "$$1" "$${1%.c}.bak"' _ {} \;
endef

define trans_add_c
	find . -type f -name "main.bak" -exec sh -c 'mv "$$1" "$${1%.bak}.c"' _ {} \;
endef

# Logging macros for Makefile usage (revamped colorful format)
LOG_PREFIX ?= configure
STATE_INFO  = ${BRIGHT_CYAN}INFO${RESET}
STATE_WARN  = ${YELLOW}WARN${RESET}
STATE_OK    = ${BRIGHT_GREEN}OK${RESET}
STATE_NOTE  = ${MAGENTA}NOTE${RESET}
STATE_ERROR = ${BRIGHT_RED}ERROR${RESET}
STATE_DEBUG = ${MAGENTA}DEBUG${RESET}

# $(call logging,STATE_TOKEN,Message)
# Produces: configure [STATE] : Message (colorized)
define logging
	printf "${BRIGHT_CYAN}${BOLD}%s${RESET} [${BOLD}%b${RESET}] : %b\n" "$(LOG_PREFIX)" "$(1)" "$(2)"
endef

define log_info
	$(call logging,$(STATE_INFO),$(1))
endef

define log_warn
	$(call logging,$(STATE_WARN),$(1))
endef

define log_ok
	$(call logging,$(STATE_OK),$(1))
endef

define log_note
	$(call logging,$(STATE_NOTE),$(1))
endef

define log_error
	$(call logging,$(STATE_ERROR),$(1))
endef

define log_debug
	$(call logging,$(STATE_DEBUG),$(1))
endef

# ============================================================================ #
#									INCLUDES								   #
# ============================================================================ #
# Library name
NAME = libft.a
MINILIBX_DIR ?= minilibx-linux
MINILIBX_LIB ?= $(MINILIBX_DIR)/libmlx.a
MLX_ALT_LIB ?= $(MINILIBX_DIR)/mlx.a
MLX_FLAGS ?=

# Detect availability of MiniLibX (directory + makefile)
MLX_ENABLED	:= 0
ifneq ($(wildcard $(MINILIBX_DIR)),)
ifneq ($(or $(wildcard $(MINILIBX_DIR)/Makefile),$(wildcard $(MINILIBX_DIR)/makefile)),)
MLX_ENABLED	:= 1
endif
endif

MAKEFLAGS		= --no-print-directory
# Tag directories that depend on MiniLibX
MLX_TAG_DIRS	?= classes/render/window classes/render/inputHandler classes/render/event

# Allow users to force-skip directories externally
SKIP_DIRS ?=
ifeq ($(MLX_ENABLED),0)
SKIP_DIRS += $(MLX_TAG_DIRS)
endif

# Directories
SRC_DIRS = 	data_structures/queue data_structures/circular_linked_list data_structures/doubly_linked_list\
					data_structures/lists data_structures/vector classes/render/map classes/render/window classes/render/inputHandler classes/render/point classes/render/line classes/trace/error \
					stdlib/arena stdlib/pool stdlib/slab classes/render/event \
					math/geometry math/geometry/draw math/geometry/effects math/geometry/matrices/ math/geometry/move math/geometry/quaternion \
					math/geometry/shapes math/geometry/view math/algebra time \
					ctype debug math memory render stdio stdio/ft_printf/src stdlib strings sort classes/render/inputHandler stdlib/strto

# Compute effective source directories after excluding tagged ones
SRC_DIRS_EFF := $(filter-out $(SKIP_DIRS),$(SRC_DIRS))

# Source files from all directories including ft_printf and gnl
SRCS		= $(foreach dir,$(SRC_DIRS_EFF),$(wildcard $(dir)/*.c))
PRINTF_SRCS	= $(shell find stdio/ft_printf/src -name "*.c" 2>/dev/null)
GNL_SRCS	= $(wildcard stdio/gnl/*.c)
ALL_SRCS	= $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS)
# Exclude every main.c from the library to avoid multiple definition
ALL_SRCS	:= $(filter-out %/main.c,$(ALL_SRCS))

# Object files with directory structure preserved
OBJS		= $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))

# Automatic dependency files (.d) alongside objects
DEPS		= $(OBJS:.o=.d)
-include $(DEPS)

# Header files
HEADERS = $(wildcard *.h)
# All headers recursively (any change forces full object rebuild as requested)
GLOBAL_HEADERS := $(shell find . -type f -name '*.h')
# All sources recursively (force full rebuild if any source changes as requested)
GLOBAL_SRCS := $(ALL_SRCS)

# Find all unique directories containing .h files in libft, always include root dir
HEADER_DIRS := $(shell find . -type f -name '*.h' -exec dirname {} \; | sort -u)
HEADER_DIRS += .
INCLUDE_FLAGS := $(foreach dir,$(HEADER_DIRS),-I$(dir))

# Add INCLUDE_FLAGS to CFLAGS
CFLAGS += $(INCLUDE_FLAGS) -g3

# Progress/animation settings
PROGRESS_WIDTH ?= 30
COUNTER_FILE := $(OBJ_DIR)/.build_count
SPIN_FILE := $(OBJ_DIR)/.spin_state
TOTAL_OBJS := $(words $(OBJS))

# Default rule: build the library
ifeq ($(MLX_ENABLED),1)
all: $(MINILIBX_LIB) $(NAME)
else
all: $(NAME)
endif

# ============================================================================ #
#									BUILD TARGETS							   #
# ============================================================================ #

both: $(NAME) libft.so
# ============================================================================ #
#									STATIC LIBRARY							   #
# ============================================================================ #

all: $(NAME)
# ============================================================================ #
#									SHARED LIBRARY							   #
# ============================================================================ #

shared: libft.so

# ============================================================================ #
#									UTILS TARGET							   #
# ============================================================================ #

# Build MiniLibX only if present; otherwise skip gracefully
$(MINILIBX_LIB):
	@$(call log_info,🔧 Building MiniLibX...)
	@if [ -d "$(MINILIBX_DIR)" ] && { [ -f "$(MINILIBX_DIR)/Makefile" ] || [ -f "$(MINILIBX_DIR)/makefile" ]; }; then \
		$(MAKE) -C $(MINILIBX_DIR) $(MLX_FLAGS) || true; \
		if [ ! -f "$(MINILIBX_LIB)" ] && [ -f "$(MLX_ALT_LIB)" ]; then \
			cp "$(MLX_ALT_LIB)" "$(MINILIBX_LIB)"; \
			$(call log_warn,[MiniLibX] Fallback: copied mlx.a -> libmlx.a); \
		fi; \
	else \
		$(call log_warn,[MiniLibX] Skipping (not present)); \
	fi

progress_init:
	@mkdir -p $(OBJ_DIR)
	@rm -f $(COUNTER_FILE) $(SPIN_FILE)
	@echo 0 > $(COUNTER_FILE)
	@echo 0 > $(SPIN_FILE)
	@$(call log_info,🚀 Starting build of $(NAME) ($(TOTAL_OBJS) objects))

# Rule to build the library
$(NAME): progress_init $(OBJS)
	@{ \
		count=$$( [ -f "$(COUNTER_FILE)" ] && cat "$(COUNTER_FILE)" || echo 0 ); \
		total=$(TOTAL_OBJS); width=$(PROGRESS_WIDTH); percent=100; done_chars=$$width; bar_done=$$(printf "%0.s#" $$(seq 1 $$done_chars)); \
		printf "\r$(BRIGHT_CYAN)[✔]$(RESET) $(DIM)building$(RESET) $(YELLOW)%-30s$(RESET) $(BRIGHT_GREEN)[%s] %3d%%$(RESET) (%d/%d)\n" "finalizing" "$$bar_done" "$$percent" "$$total" "$$total"; \
	}
	@$(call log_info,📦 Archiving library...)
	@$(AR) $(NAME) $(OBJS)
	@$(call log_ok,✓ $(NAME) created successfully!)

# Object compilation rule with live spinner and inline progress bar
# Add a trap signal to avoid infinite compilation
$(OBJ_DIR)/%.o: %.c $(GLOBAL_HEADERS) $(GLOBAL_SRCS)
	@mkdir -p $(dir $@)
	@{ \
		# Start a spinner while compiling this file \
		i=0; \
		spinner() { \
			i=0; \
			while :; do \
				case $$((i % 4)) in \
					0) sym='|';; 1) sym='/';; 2) sym='-';; 3) sym='\\';; \
				esac; \
				printf "\r$(BRIGHT_CYAN)[%s]$(RESET) $(DIM)compiling$(RESET) $(YELLOW)%-30s$(RESET)" "$$sym" "$(notdir $<)"; \
				i=$$((i + 1)); \
				sleep 0.1; \
			done; \
		}; \
		spinner & SPIN_PID=$$!; \
		trap 'kill -9 $$SPIN_PID >/dev/null 2>&1; wait $$SPIN_PID 2>/dev/null; exit 130' INT TERM EXIT; \
		# Compile with automatic dependency generation (-MMD -MP creates .d file next to .o) \
		$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -MMD -MP -c $< -o $@; STATUS=$$?; \
		trap - INT TERM EXIT; \
		kill -9 $$SPIN_PID >/dev/null 2>&1 || true; \
		wait $$SPIN_PID 2>/dev/null || true; \
		# Progress accounting \
		count=0; [ -f "$(COUNTER_FILE)" ] && count=$$(cat "$(COUNTER_FILE)"); \
		count=$$((count + 1)); echo $$count > "$(COUNTER_FILE)"; \
		total=$(TOTAL_OBJS); \
		[ $$total -eq 0 ] && total=1; \
		percent=$$((100 * count / total)); \
		# Clamp percent to [0..100] visually \
		if [ $$percent -gt 100 ]; then percent=100; fi; \
		if [ $$percent -lt 0 ]; then percent=0; fi; \
		width=$(PROGRESS_WIDTH); \
		done_chars=$$((percent * width / 100)); \
		[ $$done_chars -gt $$width ] && done_chars=$$width; \
		bar_done=$$(printf "%0.s#" $$(seq 1 $$done_chars)); \
		bar_pad=$$(printf "%0.s " $$(seq 1 $$((width - done_chars)))); \
		if [ $$STATUS -eq 0 ]; then \
			printf "\r$(BRIGHT_CYAN)[✔]$(RESET) $(DIM)building$(RESET) $(YELLOW)%-30s$(RESET) $(BRIGHT_GREEN)[%s%s] %3d%%$(RESET) (%d/%d)" \
				"$(notdir $<)" "$$bar_done" "$$bar_pad" "$$percent" "$$count" "$$total"; \
		else \
			printf "\r$(BRIGHT_RED)[✗]$(RESET) $(YELLOW)%-30s$(RESET) $(BRIGHT_RED)FAILED$(RESET)\n" "$(notdir $<)"; \
			exit $$STATUS; \
		fi; \
	}

build :
	@call(build_lib($(MINILIBX), all));
	
# Enhanced clean with visual feedback
clean:
	@$(call log_info,🧹 Cleaning object files...)
	@if [ -d "$(OBJ_DIR)" ]; then \
		$(call log_warn,Removing $(OBJ_DIR)/ directory...); \
		$(RM) -r $(OBJ_DIR); \
		$(call log_ok,✅ Object files cleaned); \
	else \
		$(call log_warn,No object files to clean); \
	fi

# Enhanced fclean with visual feedback
fclean: clean
	@$(call log_info,🔥 Deep cleaning...)
	@if [ -f "$(NAME)" ]; then \
		$(call log_warn,Removing library $(NAME)...); \
		$(RM) $(NAME); \
		$(call log_ok,✅ Library removed); \
	else \
		$(call log_warn,No library to remove); \
	fi
	@$(call log_info,🔥 Deep cleaning MiniLibX...)
	@if [ -d "$(MINILIBX_DIR)" ] && { [ -f "$(MINILIBX_DIR)/Makefile" ] || [ -f "$(MINILIBX_DIR)/makefile" ]; }; then \
		$(MAKE) -C $(MINILIBX_DIR) clean || true; \
	else \
		$(call log_warn,Skipping MiniLibX deep clean (not present)); \
	fi

# Rebuild
re: fclean all

# Debug mode with enhanced feedback
debug: CFLAGS += -g3 -fsanitize=address

debug: 
	@$(call log_warn,🐛 DEBUG MODE ENABLED)
	@$(call log_info,Flags: $(CFLAGS))
	@$(MAKE) re --no-print-directory
	@$(call log_info,🔍 Debug build completed with AddressSanitizer)

# Build a test executable from a main.c in a subdirectory
# Usage: make test TEST=window
TEST ?=

ifeq ($(TEST),)
TEST_MAIN :=
else
TEST_MAIN := $(shell find . -type f -path "*/$(TEST)/main.c" | head -n 1)
endif

# Extract just the last part of TEST for the executable name
TEST_EXE_NAME := $(notdir $(TEST))
# Place test executables in bin/ directory
BIN_DIR := bin
TEST_EXE := $(BIN_DIR)/$(TEST_EXE_NAME)

mode_42:
	@$(trans_remove_c)

mode_lab:
	@$(trans_add_c)

test: $(TEST_EXE)

norminette:
	@output="$$(find . -path "./minilibx-linux" -prune -o -name "*.c" -exec norminette {} +)"; \
	if echo "$$output" | grep -q "Error"; then \
	    echo "$$output" | grep "Error"; \
	    $(call log_error,❌ Norminette errors detected!); \
	else \
	    $(call log_ok,✅ All files passed norminette!); \
	fi

# Optional link flags for tests (only when MiniLibX is available)
ifeq ($(MLX_ENABLED),1)
LINK_MLX := $(MINILIBX_LIB) -lX11 -lXext -lm
else
LINK_MLX :=
endif

$(TEST_EXE): $(MINILIBX_LIB) $(NAME)
	@if [ -z "$(TEST_MAIN)" ]; then \
		echo "No main.c found for test '$(TEST)'"; \
		exit 1; \
	fi
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(TEST_MAIN) $(NAME) $(LINK_MLX)

libft.so: $(OBJS)
	@echo "$(BRIGHT_MAGENTA)🔗 Linking shared library libft.so...$(RESET)"
	@$(CC) $(CFLAGS_SHARED) -o libft.so $(OBJS)
	@echo "$(BRIGHT_GREEN)✓ libft.so created successfully!$(RESET)"

test_ctype:
	$(MAKE) -C ctype/ test
	./ctype/test_ctype

help:
	@$(call log_info,Makefile targets:)
	@echo "  all (default)      : Build the static library libft.a"
	@echo "  shared             : Build the shared library libft.so"
	@echo "  both               : Build both static and shared libraries"
	@echo "  clean              : Remove object files"
	@echo "  fclean             : Remove object files and libraries"
	@echo "  re                 : Rebuild the library from scratch"
	@echo "  debug              : Build the library with debug flags and AddressSanitizer"
	@echo "  test TEST=<name>   : Build and run test executable from directory <name>"
	@echo "  norminette         : Run norminette on all .c files (excluding minilibx)"
	@echo "  mode_lab           : Rename all main.bak files to main.c (for 42 labs)"
	@echo "  mode_42            : Rename all main.c files to main.bak (for 42 projects)"
	@echo "  help               : Show this help message"

.PHONY: both shared all norminette test mode_lab mode_42 test $(TEST_EXE) progress_init debug fclean clean re build