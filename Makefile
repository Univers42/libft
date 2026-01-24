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
PICFLAG = -fPIC
CFLAGS += $(PICFLAG)

BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
META_DIR = $(BUILD_DIR)/metadata

NAME = $(LIB_DIR)/libft.a
STATIC_LIB = $(LIB_DIR)/libft.a
SHARED_LIB = $(LIB_DIR)/libft.so

SRCS = $(shell find ./srcs -name "*.c")
INCS = -I./include -I.
AR = ar rcs
RM = rm -rf
TOTAL = $(words $(SRCS))
DEPFLAGS = -MMD -MP -MF
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS = $(SRCS:%.c=$(META_DIR)/%.d)

all : $(STATIC_LIB) $(SHARED_LIB)
	@printf "\r\033[K" >&2
	@printf "\n" >&2
	@printf "  \033[1;33m●\033[0m \033[1;33mlibft\033[0m \033[33m─\033[0m \033[32mcompilation successful\033[0m\n" >&2
	@printf "  \033[33m────────────────────────────────────────\033[0m\n" >&2
	@printf "  \033[32m✓\033[0m \033[37m%s\033[0m\n" "$(STATIC_LIB)" >&2
	@printf "  \033[32m✓\033[0m \033[37m%s\033[0m\n" "$(SHARED_LIB)" >&2
	@printf "\n" >&2

$(STATIC_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	@$(AR) $@ $^

$(SHARED_LIB): $(OBJS)
	@mkdir -p $(LIB_DIR)
	@$(CC) -shared -o $@ $^

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@meta_file=$(META_DIR)/$<; meta_file=$${meta_file%.c}.d; mkdir -p "$$(dirname "$$meta_file")"; \
	if ! $(CC) $(CFLAGS) $(INCS) $(DEPFLAGS) "$$meta_file" -c $< -o $@; then \
	    printf "\n\033[1;31m  ⬢ SYSTEM ERROR ⬢\033[0m\n  \033[90m▸\033[0m \033[37m%s\033[0m\n\n" "$<" >&2; \
	    exit 1; \
	fi
	@mkdir -p $(META_DIR)
	@find . -maxdepth 1 -type f -name ".*" ! -name ".gitignore" ! -name ".gitattributes" ! -name ".git" ! -name "." ! -name ".." -print0 | xargs -0 -r -I{} mv {} $(META_DIR) 2>/dev/null || true
	@count=$$(find $(OBJ_DIR) -name "*.o" 2>/dev/null | wc -l); \
	percent=$$(($$count * 100 / $(TOTAL))); \
	filled=$$(($$percent * 30 / 100)); \
	bar=""; \
	for i in $$(seq 1 30); do \
	    if [ $$i -le $$filled ]; then \
	        bar="$$bar\033[1;36m▰\033[0m"; \
	    else \
	        bar="$$bar\033[90m▱\033[0m"; \
	    fi; \
	done; \
	spin=$$(($$count % 8)); \
	case $$spin in \
	    0) spinner="\033[1;35m⠋\033[0m";; \
	    1) spinner="\033[1;36m⠙\033[0m";; \
	    2) spinner="\033[1;33m⠹\033[0m";; \
	    3) spinner="\033[1;31;5m⠸\033[0m";; \
	    4) spinner="\033[1;32m⠼\033[0m";; \
	    5) spinner="\033[1;34m⠴\033[0m";; \
	    6) spinner="\033[1;37m⠦\033[0m";; \
	    7) spinner="\033[1;96m⠧\033[0m";; \
	esac; \
	printf "\r  $$spinner \033[1;37mLIBFT\033[0m  %b  \033[1;36m%3d%%\033[0m \033[90m%d/%d\033[0m " \
	    "$$bar" $$percent $$count $(TOTAL) >&2;

clean:
	@$(RM) $(OBJ_DIR)
	
fclean : clean
	@$(RM) $(STATIC_LIB) $(SHARED_LIB) $(BUILD_DIR)

re : fclean all

verif_norm:
	@printf "\n  \033[1;36m▸\033[0m Running norminette" >&2; \
	output="$$( \
	    norminette 2>&1 | grep -v 'OK!' | grep -v 'US' & \
	    pid=$$!; \
	    while kill -0 $$pid 2>/dev/null; do \
	        for dots in '.' '..' '...' '....' '.....' '......'; do \
	            printf "\r  \033[1;36m▸\033[0m Running norminette\033[1;35m%-6s\033[0m" "$$dots" >&2; \
	            sleep 0.1; \
	            kill -0 $$pid 2>/dev/null || break; \
	        done; \
	    done; \
	    wait $$pid)"; \
	if [ -z "$$output" ]; then \
	    printf "\r\033[K  \033[1;32m✓\033[0m \033[1;37mNORM CHECK PASSED\033[0m\n\n"; \
	else \
	    printf "\r\033[K  \033[1;31m✗\033[0m \033[1;37mNORM VIOLATIONS:\033[0m\n\n\033[37m%s\033[0m\n\n" "$$output"; \
	fi

.PHONY: all fclean clean re

-include $(DEPS)
