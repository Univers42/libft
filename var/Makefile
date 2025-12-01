# project-specific settings (variables kept overrideable)
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I../strings -I./ft_printf/include
DEPFLAGS ?= -MMD -MP
NAME_LIB	= 		libvar
NAME		= 		$(NAME_LIB).a
LIB_DEPS		?= readline sort std string ctype memory std stdio
# Provide short-name(s) for this library so common.mk can register them
PROVIDES	?= string stdio
SRCDIR := .
# find all .c files recursively, strip leading ./ if present
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c')))
OBJ_DIR		?=		obj
# map src/foo/bar.c -> obj/foo/bar.o
OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
# corresponding dependency files next to objs
DPDC		:=	$(OBJS:.o=.d)
TEST_DIR	=		unit_test
TEST_SRCS	=	$(TEST_DIR)/test.c
TEST_OBJS	=	$(addprefix $(OBJ_DIR)/, $(TEST_SRCS:%.c=%.o))
TEST_BIN	=		test_$(NAME_LIB)

# Ensure dependency files are included if present (keep after DPDC definition)
-include $(DPDC)

# Now include common rules (after we defined SRCS/OBJS/DPDC so common.mk
# works with the correct lists). 
include ../mk/common.mk
