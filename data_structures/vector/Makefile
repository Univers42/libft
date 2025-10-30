CC ?= cc
CFLAGS ?= -Wall -Wextra -Werror -I. -I../../include
AR ?= ar
ARFLAGS ?= rcs
RANLIB ?= ranlib

SRCS := $(wildcard *.c)
LIB_SRCS := $(filter-out main.c,$(SRCS))

OBJDIR := obj
BINDIR := bin

LIB_OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(LIB_SRCS))
MAIN_OBJ := $(OBJDIR)/main.o
LIB := libarray.a
TARGET := $(BINDIR)/test_array

.PHONY: all clean fclean re

all: $(LIB) $(TARGET)

$(LIB): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

# ensure bin dir exists then link executable into $(BINDIR)
$(TARGET): $(MAIN_OBJ) $(LIB) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJ) $(LIB)

# ensure obj dir exists then compile each source into obj/%.o
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# create obj and bin directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	-rm -f $(LIB_OBJS) $(MAIN_OBJ)

fclean: clean
	-rm -f $(LIB) $(TARGET)
	-rmdir --ignore-fail-on-non-empty $(OBJDIR) || true
	-rmdir --ignore-fail-on-non-empty $(BINDIR) || true

re: fclean all
