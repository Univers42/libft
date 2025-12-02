# Minimal, refactor-friendly Makefile with pattern rules and automatic sources.

CXX := g++
CC  := gcc
RM  := rm -f

CXXFLAGS := -std=c++23 -g -O0 -fno-omit-frame-pointer -pthread
CFLAGS   := -std=gnu11 -g -O0
# add -rdynamic so addr2line can resolve symbols from the executable
LDFLAGS  := -rdynamic -ldl -pthread

SRCDIR := .
# build a.out by default so ./a.out matches user's run
TARGET := a.out

# automatic discovery
SRCS_CPP := $(wildcard $(SRCDIR)/*.cpp)
SRCS_C   := $(wildcard $(SRCDIR)/*.c)

# object names include extension to avoid collisions (e.g. main.c.o vs main.cpp.o)
OBJS_CPP := $(patsubst $(SRCDIR)/%.cpp,%.cpp.o,$(SRCS_CPP))
OBJS_C   := $(patsubst $(SRCDIR)/%.c,%.c.o,$(SRCS_C))
OBJS := $(OBJS_CPP) $(OBJS_C)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# compile C++ source to object (keeps .cpp.o suffix)
%.cpp.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# compile C source to object (keeps .c.o suffix)
%.c.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
