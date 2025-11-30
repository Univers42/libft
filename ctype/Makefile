# project-specific settings (variables kept overrideable)
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I../ctype -I./ft_printf/include
DEPFLAGS ?= -MMD -MP

NAME_LIB	=		libctype
NAME		=		$(NAME_LIB).a

# ctype may be independent, adjust if it needs lower-level libs
LIB_DEPS	?=

# Provide short-name(s) so common.mk will register libctype.a in BUILD_DIR
PROVIDES	?=	ctype

SRCDIR := .
# collect all .c files under the ctype tree
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c')))

OBJ_DIR		?=	obj

OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)

# include dependency files if present
-include $(DPDC)

# include common rules after variables so common.mk uses our SRCS/OBJS/DPDC
include ../mk/common.mk
