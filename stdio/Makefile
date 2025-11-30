# project-specific settings (variables kept overrideable)
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I./output -I./format -I./input -I./ft_printf/include
DEPFLAGS ?= -MMD -MP

NAME_LIB	=		libstdio
NAME		=		$(NAME_LIB).a

# stdio may depend on memory; adjust if needed
LIB_DEPS	?=	memory

# Provide short-name(s) so common.mk will register libstdio.a in BUILD_DIR
PROVIDES	?=	stdio

SRCDIR := .
# collect all .c files under the stdio tree
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c')))

OBJ_DIR		?=	obj

OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)

# include dependency files if present
-include $(DPDC)

# include common rules after variables so common.mk uses our SRCS/OBJS/DPDC
include ../mk/common.mk