CXX := g++
CC  := gcc

CXXFLAGS := -std=c++17 -O2 -Wall -pthread
CFLAGS   := -std=c11 -O2 -Wall -pthread

SRCDIR := .
OUT := c_test

.PHONY: all clean

all: $(OUT)

# compile C test
test.o: test.c
	$(CC) $(CFLAGS) -I.. -c $< -o $@

# compile C++ assertion implementation
assert.o: assert.cpp
	$(CXX) $(CXXFLAGS) -I.. -c $< -o $@

# compile logger (used for Message / printing helpers)
../log/logger.o: ../log/logger.cpp
	$(CXX) $(CXXFLAGS) -I.. -c $< -o $@

$(OUT): test.o assert.o ../log/logger.o
	$(CXX) $(CXXFLAGS) $^ -I.. -o $@

clean:
	-rm -f test.o assert.o ../log/logger.o $(OUT)
