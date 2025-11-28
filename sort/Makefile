# If you change SRCS, always run 'make fclean' before 'make' or 'make test'

include ../build/common.mk
include ../build/debug.mk

# project-specific settings
CFLAGS += -std=c99 -pedantic -Wshadow -I../include
DEPFLAGS = -MMD -MP

NAME_LIB	= 		libsort
NAME		= 		$(NAME_LIB).a
SRCS		=		$(wildcard *.c)

OBJ_DIR		=		obj
TEST_DIR	=		tests

# discover all test sources and map them to bin/<name>
TEST_SRCS	=		$(wildcard $(TEST_DIR)/*.c)
BIN_DIR		=		bin
TEST_BINS	=		$(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%,$(TEST_SRCS))

# ----------------- dependency support -----------------
# list dependency repositories (directories) that contain their own Makefile
DEP_REPOS	=		../memory
# list the library names produced by those repos (basename of repo)
DEP_LIBS	=		$(patsubst %/,%,$(notdir $(DEP_REPOS)))
DEP_DIR		=		dependency
DEP_LIB_A	=		$(addprefix $(DEP_DIR)/,$(addsuffix .a,$(DEP_LIBS)))
# -------------------------------------------------------

OBJS		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
DPDC		=		$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.d))

LIBS		=		memory.a string.a

.PHONY: all clean fclean re test bins deps

all: $(DPDC) $(NAME)

$(NAME): $(DPDC) $(OBJS)
	$(call print_status,$(MAGENTA),ARCHIVE,Creating $(NAME)...)
	@$(AR) $(ARFLAGS) $@ $^ >/dev/null 2>&1
	$(call print_status,$(GREEN),DONE,Library $(NAME) successfully created!)

$(LIBS):
	find ../ -name "$<" ## if we found them we don't run their Make otherwise we compile the libs
	$(MAKE) $<
$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@$(call create_dirs)
	@mkdir -p $(dir $@)
	@$(call print_status,$(CYAN),COMPILE,Compiling $< -> $@)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(OBJ_DIR):
	@$(call create_dirs)

$(OBJ_DIR)/$(TEST_DIR):
	@mkdir -p $(OBJ_DIR)/$(TEST_DIR)

# Build each test program into bin/<name>
# Each test source is compiled and linked against the static library $(NAME)
# and any dependency libraries present in $(DEP_DIR)
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(NAME) deps
	@mkdir -p $(BIN_DIR)
	@$(call print_status,$(CYAN),COMPILE,Compiling test $< -> $@)
	@$(CC) $(CFLAGS) -I. $< $(NAME) $(DEP_LIB_A) -o $@

# Test target builds all test binaries (ensures deps are prepared)
test: $(NAME) bins
	@$(call print_status,$(BLUE),TEST,All test binaries ready in $(BIN_DIR)/)
	@echo "Run $(BIN_DIR)/* to execute tests."

# alias rule to build all bins
bins: $(TEST_BINS)

# ------------------- dependency rules -------------------
# Build dependencies and copy produced .a files into dependency/
deps: $(DEP_DIR)
	@set -e; \
	for repo in $(DEP_REPOS); do \
		echo "Building dependency $$repo"; \
		$(MAKE) -C $$repo || exit 1; \
		echo "Collecting archives from $$repo"; \
		# POSIX-safe copy of all .a files from dependency repo into DEP_DIR
		find "$$repo" -type f -name '*.a' -exec cp -f {} "$(DEP_DIR)/" \; || exit 1; \
	done

$(DEP_DIR):
	@mkdir -p $(DEP_DIR)
# -------------------------------------------------------

# convenience: build a single test by name:
# make bin/yourtest  (requires tests/yourtest.c to exist)

clean:
	$(call clean_objects)
	@$(call print_status,$(YELLOW),CLEAN,Removing test binaries and dependencies...)
	@rm -rf $(BIN_DIR) $(DEP_DIR)

fclean: clean
	$(call print_status,$(RED),CLEAN,Removing archive...)
	@$(RM) $(NAME)

re: fclean all

-include $(DPDC)