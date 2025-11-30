include ../mk/common.mk
include ../mk/debug.mk

# project-specific settings
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I./ft_printf/include \
	-I./output -I./input -I./format

DEPFLAGS = -MMD -MP

NAME_LIB	= 		libstdio
NAME		= 		$(NAME_LIB).a

# new: location in the root libft build directory where the .a will be placed
LIB_ROOT	:=	$(abspath ..)
LIB_BUILD_DIR:=	$(LIB_ROOT)/build
OUT_LIB		:=	$(LIB_BUILD_DIR)/$(NAME)

# Gather all .c files in output and format (input sources intentionally excluded for now)
SRCS		+=		$(wildcard output/*.c)
SRCS		+=		$(wildcard format/*.c)

OBJ_DIR		=		obj
TEST_DIR	=		tests

TEST_SRCS	:=	$(wildcard $(TEST_DIR)/*.c)
TEST_BINS	:=	$(patsubst $(TEST_DIR)/%.c,bin/%,$(TEST_SRCS))

VPATH = .:output:format:gnl:ft_printf/src

.PHONY: all clean fclean re test

# build the library into the central build dir (create real lib here, symlink in build/)
all: $(OUT_LIB)

# create the real archive locally (stdio/libstdio.a)
$(NAME): $(OBJS)
	$(call print_status,$(MAGENTA),ARCHIVE,Creating $(NAME)...)
	@$(AR) $(ARFLAGS) $@ $^ >/dev/null 2>&1
	$(call print_status,$(GREEN),DONE,Library $(NAME) successfully created!)

# create a symlink in the central build dir that points to the local archive
$(OUT_LIB): $(NAME)
	$(call print_status,$(MAGENTA),LINK,Creating symlink $(OUT_LIB) -> $(abspath $(NAME))...)
	@mkdir -p $(LIB_BUILD_DIR)
	@rm -f $(OUT_LIB)
	@ln -sf $(abspath $(NAME)) $(OUT_LIB)
	$(call print_status,$(GREEN),DONE,Symlink $(OUT_LIB) -> $(abspath $(NAME)) created!)

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

# Test target — ensure the build symlink (OUT_LIB) is present and link against it
test: $(TEST_BINS)
	$(call print_status,$(BLUE),TEST,Built $(words $(TEST_BINS)) test binaries in bin/)
	@echo "Run ./bin/<name> to execute individual tests."

# rule: build bin/<testname> from tests/<testname>.c linking against OUT_LIB
bin/%: $(TEST_DIR)/%.c | bin $(OUT_LIB)
	$(Q)$(call print_status,$(CYAN),COMPILE+LINK,Building $@ from $<)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) -I. -I../include $< $(OUT_LIB) -o $@

bin:
	$(Q)mkdir -p bin

clean:
	$(call clean_objects)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive and test binary...)
	@$(RM) $(OUT_LIB) $(NAME) $(TEST_BIN)

re: fclean all

-include $(DPDC)