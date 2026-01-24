# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/10 19:03:45 by marvin            #+#    #+#              #
#    Updated: 2026/01/10 19:03:45 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# voluntarily Makefile is simpler to debug quicker the problems on this libft..

CC = cc
CFLAGS = -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -DMINISHELL_DEBUG_PROMPT=1 -std=c99

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
META_DIR = $(BUILD_DIR)/metadata

NAME = $(LIB_DIR)/libft.a

SRCS = $(shell find ./srcs -name "*.c")
INCS = -I./include -I. -I./include/internals
AR = ar rcs
RM = rm -rf
TOTAL = $(words $(SRCS))
DEPFLAGS = -MMD -MP -MF
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS = $(SRCS:%.c=$(META_DIR)/%.d)

all : $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(LIB_DIR)
	@echo -e "\nDone."
	@$(AR) $@ $^ 

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@meta_file=$(META_DIR)/$<; meta_file=$${meta_file%.c}.d; mkdir -p "$$(dirname "$$meta_file")"; \
	if ! $(CC) $(CFLAGS) $(INCS) $(DEPFLAGS) "$$meta_file" -c $< -o $@; then \
		echo -e "\n\033[31mError compiling $<\033[0m" >&2; \
		exit 1; \
	fi
	@mkdir -p $(META_DIR)
	@find . -maxdepth 1 -type f -name ".*" ! -name ".gitignore" ! -name ".gitattributes" ! -name ".git" ! -name "." ! -name ".." -print0 | xargs -0 -r -I{} mv {} $(META_DIR) 2>/dev/null || true
	@count=$$(find $(OBJ_DIR) -name "*.o" 2>/dev/null | wc -l); \
	printf "\r\033[32m%d / $(TOTAL)\033[0m" $$count >&2; \
	echo -n "" >&2;

clean:
	$(RM) $(OBJ_DIR)
	
fclean : clean
	$(RM) $(NAME) $(BUILD_DIR)

re : fclean all

.PHONY: all fclean clean re

-include $(DEPS)
