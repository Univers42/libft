# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

CC			= 		cc
CFLAGS		= 		-Wall -Wextra -Werror -pedantic -Wshadow -std=c99 -I../include -I./ft_printf/include
DEPFLAGS	= 		-MMD -MP
NAME_LIB	= 		string
NAME		= 		$(NAME_LIB).a		
SRCS		=  		ft_atol.c ft_countwords.c ft_split.c ft_strchr.c ft_strclen.c \
					ft_strcmp.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
					ft_strlcpy.c ft_strmapi.c ft_strncmp.c ft_strncpy.c ft_strndup.c \
					ft_strnjoin.c ft_strnstr.c ft_strtok.c ft_substr.c ft_strtrim.c \
					ft_substr.c ft_super_trim.c helper_super_trim.c

OBJ_DIR		=		obj
TEST_DIR	=		unit_test

TEST_SRCS	=		$(TEST_DIR)/test.c
TEST_OBJS	=		$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=		test_$(NAME_LIB)

OBJS		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))
RM			:=		rm -rf
AR			:=		ar rcs

all: $(NAME)

$(NAME):$(OBJS)
	$(AR) $@ $^

$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/$(TEST_DIR):
	mkdir -p $(OBJ_DIR)/$(TEST_DIR)

# Build test object
$(OBJ_DIR)/$(TEST_DIR)/%.o : $(TEST_DIR)/%.c | $(OBJ_DIR)/$(TEST_DIR)
	$(CC) $(CFLAGS) -I. -c $< -o $@

# Test target
test: $(NAME) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) -o $(TEST_BIN)
	@echo "Run ./$(TEST_BIN) to execute tests."

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME) $(TEST_BIN)

re: fclean all

.PHONY: all clean fclean re test

-include $(DPDC)