# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

include ../mk/common.mk
include ../mk/debug.mk

# project-specific settings
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I../strings -I./ft_printf/include
DEPFLAGS = -MMD -MP

NAME_LIB	= 		libstring
NAME		= 		$(NAME_LIB).a		
SRCS		=  		$(wildcard *.c)

OBJ_DIR		=		obj
TEST_DIR	=		unit_test

TEST_SRCS	=		$(TEST_DIR)/test.c
TEST_OBJS	=		$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=		test_$(NAME_LIB)

OBJS		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))

# Path to the test framework static library (adjust if needed)
TEST_FRAMEWORK_DIR = ../testing
TEST_FRAMEWORK_LIB = $(TEST_FRAMEWORK_DIR)/libtest_framework.a
LIBFT = ../libft.a

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJS)
	$(call print_status,$(MAGENTA),ARCHIVE,Creating $(NAME)...)
	@$(AR) $(ARFLAGS) $@ $^ >/dev/null 2>&1
	$(call print_status,$(GREEN),DONE,Library $(NAME) successfully created!)

$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@$(call create_dirs)
	@mkdir -p $(dir $@)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(call create_dirs)

$(OBJ_DIR)/$(TEST_DIR):
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)

# Build test object
$(OBJ_DIR)/$(TEST_DIR)/%.o : $(TEST_DIR)/%.c | $(OBJ_DIR)/$(TEST_DIR)
	@$(call create_dirs)
	@mkdir -p $(dir $@)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) -I. -c $< -o $@

# Test target
test: $(NAME) $(TEST_OBJS) $(TEST_FRAMEWORK_LIB) $(LIBFT)
	$(call print_status,$(BLUE),TEST,Building test runner...)
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(OBJS) $(TEST_FRAMEWORK_LIB) $(LIBFT) -o $(TEST_BIN)
	$(call print_status,$(GREEN),TEST,Test binary ready: ./$(TEST_BIN))
	@echo "Run ./$(TEST_BIN) to execute tests."

clean:
	$(call clean_objects)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive and test binary...)
	@$(RM) $(NAME) $(TEST_BIN)

re: fclean all

-include $(DPDC)