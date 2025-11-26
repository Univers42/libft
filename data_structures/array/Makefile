CC ?= gcc
CFLAGS ?= -Wall -Wextra -Werror -I. -I../../include/
AR ?= ar
ARFLAGS ?= rcs
RANLIB ?= ranlib

SRCS := $(wildcard *.c)
LIB_SRCS := $(filter-out main.c,$(SRCS))

OBJDIR := obj
BINDIR := bin

LIB_OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(LIB_SRCS))

# tests - build every tests/*.c into bin/<name>
TEST_SRCS := $(wildcard tests/*.c)
TEST_OBJS := $(patsubst tests/%.c,$(OBJDIR)/%.o,$(TEST_SRCS))
TEST_BINS := $(patsubst tests/%.c,$(BINDIR)/%,$(TEST_SRCS))

LIB := libarray.a

# string dependency
STRING_DIR := ../../strings
STRING_LIB := $(STRING_DIR)/string.a

.PHONY: all clean fclean re asan

all: $(LIB) $(STRING_LIB) $(TEST_BINS)

$(LIB): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

# build string library if needed
$(STRING_LIB):
	$(MAKE) -C $(STRING_DIR)

# link each test executable: bin/<name>
$(BINDIR)/%: $(OBJDIR)/%.o $(LIB) $(STRING_LIB) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LIB) $(STRING_LIB)

# compile test sources into obj/<name>.o
$(OBJDIR)/%.o: tests/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ensure obj dir exists then compile each top-level source into obj/%.o
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create obj and bin directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	-rm -f $(LIB_OBJS) $(TEST_OBJS) $(OBJDIR)/*.o

fclean: clean
	-rm -f $(LIB) $(TEST_BINS)
	-rmdir --ignore-fail-on-non-empty $(OBJDIR) || true
	-rmdir --ignore-fail-on-non-empty $(BINDIR) || true

re: fclean all

asan:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -g -fsanitize=address,undefined -fno-omit-frame-pointer" all
