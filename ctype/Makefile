# project-specific settings (variables kept overrideable)
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I../ctype -I./ft_printf/include
DEPFLAGS ?= -MMD -MP

NAME_LIB	=		libctype
NAME		=		$(NAME_LIB).a

# ctype uses ft_strchr from string module
LIB_DEPS	?= string

# Provide short-name(s) so common.mk will register libctype.a in BUILD_DIR
PROVIDES	?=	ctype

SRCDIR := .
# collect ONLY .c files directly in ctype/ using -maxdepth 1
# This prevents descending into math/, render/, etc. subdirectories
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -maxdepth 1 -type f -name '*.c')))

OBJ_DIR		?=	obj

OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)

# include dependency files if present
-include $(DPDC)

# include common rules after variables so common.mk uses our SRCS/OBJS/DPDC
include ../mk/common.mk
