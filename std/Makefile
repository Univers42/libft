# project-specific settings (variables kept overrideable)
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I../std -I./ft_printf/include
DEPFLAGS ?= -MMD -MP

NAME_LIB	=		libstd
NAME		=		$(NAME_LIB).a

# std may depend on low-level utilities (adjust as needed)
LIB_DEPS	?=	memory

# Provide short-name(s) for this library so common.mk can register them
PROVIDES	?=	std

SRCDIR := .
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c')))

OBJ_DIR		?=	obj

OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)

# include dependency files if present
-include $(DPDC)

# include common rules after variables so common.mk uses our SRCS/OBJS/DPDC
include ../mk/common.mk
