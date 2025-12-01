CFLAGS		+= -std=c99 -pedantic -Wshadow -I../include -I../ctype -I./ft_printf/include
DEPFLAGS	?= -MMD -MP
NAME_LIB	=		libsort
NAME		=		$(NAME_LIB).a
LIB_DEPS	?=
PROVIDES	?=	ctype
SRCDIR := .
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c')))
OBJ_DIR		?=	obj
OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)
-include $(DPDC)

# include common rules after variables so common.mk uses our SRCS/OBJS/DPDC
include ../mk/common.mk
