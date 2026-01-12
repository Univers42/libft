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

CC = gcc
CFLAGS = -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -DMINISHELL_DEBUG_PROMPT=1
NAME = libft.a
SRCS = $(shell find ./srcs -name "*.c")
INCS = -I./include -I. -I./include/internals
OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
AR = ar rcs
RM = rm -rf
TOTAL = $(words $(SRCS))

all : $(NAME)

$(NAME): $(OBJS)
	@echo -e "\nDone."
	@$(AR) $@ $^ 

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@if ! $(CC) $(CFLAGS) $(INCS) -c $< -o $@; then \
		echo -e "\n\033[31mError compiling $<\033[0m" >&2; \
		exit 1; \
	fi
	@count=$$(find $(OBJ_DIR) -name "*.o" 2>/dev/null | wc -l); \
	printf "\r\033[32m%d / $(TOTAL)\033[0m" $$count >&2; \
	echo -n "" >&2;

clean:
	$(RM) $(OBJ_DIR)
	
fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all fclean clean re