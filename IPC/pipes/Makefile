# ...new file...

CC := gcc
CXX := g++
AR := ar
RANLIB := ranlib
MAKE_PROG := $(MAKE)

CFLAGS := -Wall -Wextra -O2 -g 
CXXFLAGS := -Wall -Wextra -O2 -g -std=c++11

# lifoba location (relative to this directory)
LIFOBADIR := ../../memory/memalloc/garbage_lifoba/lifoba
LIFOBALIB := $(LIFOBADIR)/libgarbcollector.a

# include paths (add lifoba for lifoba headers)
INC := -I. -I.. -I../.. -I$(LIFOBADIR)

OBJDIR := .obj
BINDIR := bin
LIB := libpipes.a

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

TEST_SRCS := $(wildcard tests/*.cpp)
TEST_BINS := $(patsubst tests/%.cpp,$(BINDIR)/%,$(TEST_SRCS))

.PHONY: all clean fclean re lifoba

all: $(LIB) $(LIFOBALIB) $(TEST_BINS)

# Ensure lifoba lib is available: if not present build it
$(LIFOBALIB):
	@if [ -f "$@" ]; then \
		echo "Using existing $@"; \
	else \
		echo "Building lifoba library in $(LIFOBADIR) ..."; \
		$(MAKE_PROG) -C $(LIFOBADIR) || (echo "lifoba build failed"; exit 1); \
	fi

# static library
$(LIB): $(OBJS)
	$(AR) rcs $@ $^
	-$(RANLIB) $@ || true

# compile C objects into .obj/
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# build bin dir
$(BINDIR):
	mkdir -p $(BINDIR)

# compile each test and link with the static lib and lifoba lib
$(BINDIR)/%: tests/%.cpp $(LIB) $(LIFOBALIB) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(INC) $< $(LIB) $(LIFOBALIB) -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(LIB)
	rm -rf $(BINDIR)

re: fclean all
