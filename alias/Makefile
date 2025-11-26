# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/26 01:34:08 by dlesieur          #+#    #+#              #
#    Updated: 2025/11/26 01:34:09 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Alias module Makefile, adapted to libft framework

BUILD_DIR := ../build
include $(BUILD_DIR)/common.mk

# Toolchain
CC ?= $(CC)
CFLAGS += -I../include
DEPFLAGS = -MMD -MP

NAME = alias.a

SRCS = \
	alias_cmd.c \
	alias_find_slot.c \
	alias_free.c \
	alias_lookup.c \
	alias_print.c \
	alias_remove_all.c \
	alias_set.c \
	alias_unset.c \
	alias_str_quote.c \
	get_alias_state.c \
	unalias_cmd.c

OBJ_DIR := obj
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)

TESTOBJ := $(OBJ_DIR)/test.o
TESTBIN := test_alias

TEST_FRAMEWORK_LIB := ../testing/libtest_framework.a
LIBFT := ../libft.a

.PHONY: all test clean fclean re

# Default target
all: pre_build $(NAME)

pre_build:
	$(call create_dirs)
	$(call print_status,$(CYAN),SETUP,Preparing alias build...)

# Archive library
$(NAME):
	$(call print_status,$(MAGENTA),ARCHIVE,Ensuring object files and creating $@)
	@set -e; \
	for src in $(SRCS); do \
		srcpath=$$src; \
		obj="$(OBJ_DIR)/$${srcpath%.c}.o"; \
		if [ ! -f "$$obj" ]; then \
			printf "[COMP] building %s -> %s\n" "$$srcpath" "$$obj"; \
			$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(DEPFLAGS) -c "$$srcpath" -o "$$obj"; \
		fi; \
	done;
	$(AR) $(ARFLAGS) $@ $(OBJS)
	$(call print_status,$(GREEN),DONE,$@ created)

# Compile rule
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(call print_status,$(YELLOW),COMP,Compiling $<)
	@$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(DEPFLAGS) -c $< -o $@

$(TESTBIN): $(OBJS) $(TESTOBJ)
	$(call print_status,$(MAGENTA),LINK,Linking test binary)
	@$(CC) $(CFLAGS) $^ $(TEST_FRAMEWORK_LIB) $(LIBFT) -o $@
	$(call print_status,$(GREEN),DONE,Test binary ready: $@)

 test: $(TESTBIN)
	$(call print_status,$(CYAN),TEST,Running alias tests)
	@./$(TESTBIN)

$(LIBFT):
	$(call print_status,$(YELLOW),LIB,Requesting top-level libft.a)
	@$(MAKE) -C .. libft.a

# Clean
clean:
	$(call print_status,$(RED),CLEAN,Removing object files)
	@rm -rf $(OBJ_DIR)

fclean: clean
	$(call print_status,$(RED),FCLEAN,Removing libraries and test binary)
	@rm -f $(NAME) $(TESTBIN)

re: fclean all

# Include dependency files
-include $(DEPS)
