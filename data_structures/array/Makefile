CC ?= gcc
CFLAGS ?= -Wall -Wextra -Werror -I.
AR ?= ar rcs
RANLIB ?= ranlib

SRCS := $(wildcard *.c)
LIB_SRCS := $(filter-out main.c,$(SRCS))
LIB_OBJS := $(LIB_SRCS:.c=.o)
MAIN_OBJ := main.o
LIB := libarray.a
TARGET := test_array

.PHONY: all clean fclean re

all: $(LIB) $(TARGET)

$(LIB): $(LIB_OBJS)
	$(AR) $@ $^
	$(RANLIB) $@

$(TARGET): $(MAIN_OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJ) $(LIB)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(LIB_OBJS) $(MAIN_OBJ)

fclean: clean
	-rm -f $(LIB) $(TARGET)

re: fclean all
