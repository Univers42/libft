# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

include ../mk/common.mk
include ../mk/debug.mk

# project-specific settings
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I./ft_printf/include
DEPFLAGS = -MMD -MP
LIST_LIB_DPDC	:= ../build/libstring.a ../build/stdio.a
# override build directory to place the real library into ../build
BUILD_DIR := ../build
# changed: make BUILD_OUT recursive so $(NAME) is expanded when used (NAME is defined later)
BUILD_OUT = $(abspath $(BUILD_DIR)/$(NAME))
NAME_LIB	=	libmemory
NAME		=	$(NAME_LIB).a
SRCS		=	$(filter-out test_%.c,$(wildcard *.c))
SRCS		+=	$(wildcard memalloc/garbage_collector/lifoba/*.c)
SRCS		+=	$(wildcard memalloc/garbage_collector/arena/*.c)
SRCS		+=	$(wildcard memalloc/garbage_collector/pool/*.c)
SRCS		+=	$(wildcard memalloc/garbage_collector/slab/*.c)

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

$(BUILD_DIR): 
	@mkdir -p $@

# Build ../strings and create/update a symlink in dependency/
$(DEP_LINK): | $(DEP_DIR)
	@$(MAKE) -C ../strings all
	@ln -sf $(abspath ../strings/string.a) $(DEP_LINK)
	@$(call print_status,$(CYAN),DEPS,Linked $(DEP_LINK))

# check-build-lib: updated to inspect the real build output
check-build-lib:
	@if [ -e "$(BUILD_OUT)" ]; then \
		echo "$(BUILD_OUT) is present."; \
	else \
		echo "$(BUILD_OUT) does not exist yet."; \
	fi

# Provide defaults in case included mk files don't set them
AR ?= ar
ARFLAGS ?= rcs

# replace the previous $(NAME) archive rule with a rule that writes the real archive into ../build
# (keep messages and error reporting)
$(BUILD_OUT): check-build-lib $(OBJS) | $(BUILD_DIR)
	$(call print_status,$(MAGENTA),ARCHIVE,Creating $(BUILD_OUT)...)
	@mkdir -p $(BUILD_DIR)
	@$(AR) $(ARFLAGS) $@ $(OBJS) || { echo "ERROR: archiving $(BUILD_OUT) failed"; rm -f $@; exit 1; }
	$(call print_status,$(GREEN),DONE,Library $(BUILD_OUT) successfully created!)

# Provide a lightweight phony alias so references to $(NAME) still work (if any)
.PHONY: $(NAME)
$(NAME): $(BUILD_OUT)
	@# dummy alias to satisfy targets depending on $(NAME)

# compile regular objects
$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(call create_dirs)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# compile test objects (ensure dependency symlink and library exist first)
$(OBJ_DIR)/$(TEST_DIR)/%.o : $(TEST_DIR)/%.c | $(OBJ_DIR) $(OBJ_DIR)/$(TEST_DIR) $(DEP_LINK) $(BUILD_OUT)
	@mkdir -p $(dir $@)
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
# ensure we build the real archive in ../build first
test: $(DEP_LINK) $(BUILD_OUT) $(BIN_DIR) $(TEST_BINS)
	@$(call print_status,$(GREEN),TEST,All test binaries ready in $(BIN_DIR))

# Link rule for each test binary: link test object with memory.a and dependency symlink
$(BIN_DIR)/%: $(OBJ_DIR)/$(TEST_DIR)/%.o $(BUILD_OUT) $(DEP_LINK) | $(BIN_DIR)
	@$(call print_status,$(BLUE),TEST,Linking $@ ...)
	@$(CC) $(CFLAGS) $< $(BUILD_OUT) $(DEP_LINK) -o $@
	@$(call print_status,$(GREEN),TEST,Created $@)

clean:
	$(call clean_objects)
	@rm -rf $(OBJ_DIR)/$(TEST_DIR)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive and test binaries...)
	@$(RM) $(BUILD_OUT)
	@rm -rf $(BIN_DIR)
	@rm -f $(TEST_BINS)
	@rm -rf $(DEP_DIR)

re: fclean all

.PHONY: all clean fclean re test
-include $(DEPS)