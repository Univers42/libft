# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

include ../mk/common.mk
include ../mk/debug.mk

# project-specific settings
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I./ft_printf/include
DEPFLAGS = -MMD -MP
STRINGS_LIB := ../strings/string.a

NAME_LIB	=	memory
NAME		=	$(NAME_LIB).a
SRCS		=	$(filter-out test_%.c,$(wildcard *.c))

OBJ_DIR		=	obj
TEST_DIR	=	tests
BIN_DIR		=	bin
DEP_DIR		=	dependency
DEP_LINK	=	$(DEP_DIR)/string.a

# find all test sources under tests/ and produce corresponding bin/<name> targets
TEST_SRCS	=	$(wildcard $(TEST_DIR)/*.c)
TEST_BINS	=	$(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

OBJS		=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
TEST_OBJS	=	$(addprefix $(OBJ_DIR)/$(TEST_DIR)/, $(notdir $(TEST_SRCS:.c=.o)))
DEPS		=	$(OBJS:.o=.d)

all: $(DEP_LINK) $(NAME)

# Build ../strings and create/update a symlink in dependency/
$(DEP_LINK): | $(DEP_DIR)
	@$(MAKE) -C ../strings all
	@ln -sf $(abspath ../strings/string.a) $(DEP_LINK)
	@$(call print_status,$(CYAN),DEPS,Linked $(DEP_LINK))

$(NAME): $(OBJS)
	$(call print_status,$(MAGENTA),ARCHIVE,Creating $(NAME)...)
	@$(AR) $(ARFLAGS) $@ $^ >/dev/null 2>&1
	$(call print_status,$(GREEN),DONE,Library $(NAME) successfully created!)

# compile regular objects
$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@$(call create_dirs)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# compile test objects (ensure dependency symlink and library exist first)
$(OBJ_DIR)/$(TEST_DIR)/%.o : $(TEST_DIR)/%.c | $(OBJ_DIR) $(OBJ_DIR)/$(TEST_DIR) $(DEP_LINK) $(NAME)
	@$(call create_dirs)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -DTEST_SPLIT_CPY -I. -c $< -o $@

$(OBJ_DIR):
	@$(call create_dirs)

$(OBJ_DIR)/$(TEST_DIR):
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)

# ensure bin dir exists
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# ensure dependency dir exists
$(DEP_DIR):
	@mkdir -p $(DEP_DIR)

# Build all test binaries (each test -> bin/<name>)
test: $(DEP_LINK) $(NAME) $(BIN_DIR) $(TEST_BINS)
	@$(call print_status,$(GREEN),TEST,All test binaries ready in $(BIN_DIR))

# Link rule for each test binary: link test object with memory.a and dependency symlink
$(BIN_DIR)/%: $(OBJ_DIR)/$(TEST_DIR)/%.o $(NAME) $(DEP_LINK) | $(BIN_DIR)
	@$(call print_status,$(BLUE),TEST,Linking $@ ...)
	@$(CC) $(CFLAGS) $< $(NAME) $(DEP_LINK) -o $@
	@$(call print_status,$(GREEN),TEST,Created $@)

clean:
	$(call clean_objects)
	@rm -rf $(OBJ_DIR)/$(TEST_DIR)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive and test binaries...)
	@$(RM) $(NAME)
	@rm -rf $(BIN_DIR)
	@rm -f $(TEST_BINS)
	@rm -rf $(DEP_DIR)

re: fclean all

.PHONY: all clean fclean re test
-include $(DEPS)