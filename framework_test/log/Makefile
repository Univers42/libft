CXX := g++
CC  := gcc

CXXFLAGS := -std=c++17 -O2 -Wall -pthread
CFLAGS   := -std=c11 -O2 -Wall -pthread

SRCDIR := .
OBJDIR := .

CPP_SRCS := logger.cpp
CPP_OBJS := $(CPP_SRCS:.cpp=.o)

C_SRCS := c_demo.c c_showcase.c
C_OBJS := $(C_SRCS:.c=.o)

TARGETS := c_demo c_showcase

.PHONY: all clean

all: $(TARGETS)

# Compile C++ objects
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C object
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link using C++ linker so std::string / iostream are resolved
c_demo: $(CPP_OBJS) c_demo.o
	$(CXX) $(CXXFLAGS) $^ -o $@

c_showcase: $(CPP_OBJS) c_showcase.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	-rm -f $(CPP_OBJS) $(C_OBJS) $(TARGETS) app.log
