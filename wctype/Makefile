# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/28 01:07:51 by dlesieur          #+#    #+#              #
#    Updated: 2025/11/28 01:41:47 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:=	libwctype.a
SRCS			:=	$(wildcard *.c)
OBJS			:=	$(SRCS:.c=.o)
LIB_LISTS		:=	string memory
DIRS_LIST		:=	dependencies obj bin tests

# Derive directory variables from DIRS_LIST
DEP_DIR			:=	$(word 1, $(DIRS_LIST))
OBJ_DIR			:=	$(word 2, $(DIRS_LIST))
BIN_DIR			:=	$(word 3, $(DIRS_LIST))
TEST_DIR		:=	$(word 4, $(DIRS_LIST))

CC				:= 	cc
CFLAGS			:= 	-Wall -Wextra -Werror
LDFLAGS			:= 	-I/include -I.
LIBFLAGS		:= 	$(foreach lib, $(LIB_LISTS), -L../$(lib) -l$(lib))
RM				:= 	rm -rf
AR				:= 	ar rcs

#compile the lib
all: $(DEP_DIR) $(OBJ_DIR) $(BIN_DIR) $(TEST_DIR) $(NAME)

$(DEP_DIR) $(OBJ_DIR) $(BIN_DIR) $(TEST_DIR):
	mkdir -p $@

# Generate object files and hold them into obj/
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generate the libwctype.a
$(NAME): $(addprefix $(OBJ_DIR)/, $(OBJS))  # Fixed: Depend on objects in OBJ_DIR
	$(AR) $@ $^

# Removed invalid $(LIBFLAGS) target; add build deps for libs if needed
# $(foreach lib, $(LIB_LISTS), $(MAKE) -C ../$(lib) all)  # Uncomment and adjust if you want to build libs here

# Clean all objects
clean:
	$(RM) $(OBJ_DIR)

# Clean all mkgen and lib
fclean: clean
	$(RM) $(NAME) $(DIRS_LIST)
    
test: all
	mkdir -p $(BIN_DIR)
	for file in $(TEST_DIR)/*.c; do \
		base=$$(basename $$file .c); \
		libs=""; \
		for lib in $(LIB_LISTS); do \
			if [ -f ../$$lib/lib$$lib.a ]; then \
				libs="$$libs ../$$lib/lib$$lib.a"; \
			else \
				printf "warning: dependency ../%s/lib%s.a not found, skipping\n" "$$lib" "$$lib" 1>&2; \
			fi; \
		done; \
		printf "Linking %s with: %s %s\n" "$$base" "$(NAME)" "$$libs"; \
		$(CC) $(CFLAGS) $$file $(NAME) $$libs -o $(BIN_DIR)/$$base; \
	done

re: fclean all

.PHONY: all clean fclean re $(DEP_DIR) $(OBJ_DIR) $(BIN_DIR)