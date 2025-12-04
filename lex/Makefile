# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/04 23:28:50 by dlesieur          #+#    #+#              #
#    Updated: 2025/12/05 00:13:48 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEPFLAGS ?= -MMD -MP
NAME_LIB	= 		liblex
NAME		= 		$(NAME_LIB).a

# CRITICAL: Declare ALL dependencies used by lex code
# - string: ft_strlen, ft_memcmp, ft_strchr, max_munch (singular, not "strings")
# - ctype: is_spechr, ft_isspace, ft_isalpha, ft_isdigit
# - memory: ft_memcpy, ft_memset
# - ds: deque_* functions
# - debug: ft_assert
# - stdio: ft_dprintf (used by debug/ft_assert.c)
# - std: basic utilities
LIB_DEPS		?= string ctype memory ds debug stdio std math

# Provide short-name(s) for this library so common.mk can register them
PROVIDES	?= lex

SRCDIR := .
# Exclude tests/ directory from source collection
DIR_EXCLUDE := tests

# find all .c files recursively, strip leading ./ if present
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c' ! -path '*/tests/*')))

OBJ_DIR		?=		obj
# map src/foo/bar.c -> obj/foo/bar.o
OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
# corresponding dependency files next to objs
DPDC		:=	$(OBJS:.o=.d)

# Ensure dependency files are included if present (keep after DPDC definition)
-include $(DPDC)

# Now include common rules (after we defined SRCS/OBJS/DPDC so common.mk
# works with the correct lists). 
include ../mk/common.mk

# Add a few C++ flags local to this module's test build so C headers included
# from C++ tests don't fail due to pedantic/ISO-only checks. Be conservative.
# We avoid touching global CFLAGS; only tests compiled with CXX will use these.
CXXFLAGS += -std=gnu++20 -fpermissive -Wno-error=pedantic -Wno-error=c++20-extensions

# C++ compilation rule for test sources
$(OBJ_DIR)/tests/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(USER_DEFINES) -c $< -o $@

# Test binary: compile test sources and link with TEST_FLAGS (LD_GROUP)
TEST_SRCS	:= $(wildcard tests/*.cpp)
TEST_OBJS	:= $(patsubst tests/%.cpp,$(OBJ_DIR)/tests/%.o,$(TEST_SRCS))
TEST_BIN	:= test_$(NAME_LIB)

$(TEST_BIN): $(NAME) $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) $(TEST_FLAGS) -o $@

test: $(TEST_BIN)
	./$(TEST_BIN) --test

.PHONY: test
