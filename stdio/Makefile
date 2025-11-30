CFLAGS		+= -std=c99 -pedantic -Wshadow -I../include -I./output -I./format -I./input
DEPFLAGS	?= -MMD -MP
NAME_LIB	=		libctype
NAME		=		$(NAME_LIB).a
LIB_DEPS	?= ctype string memory std
DIR_EXCLUDE	?= input
PROVIDES	?=	ctype
SRCDIR := .
DIR_EXCLUDE ?= input
OBJ_DIR		?=	obj
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -path '$(SRCDIR)/input' -prune -o -type f -name '*.c' -print)))
OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)
include $(wildcard $(DPDC))
include ../mk/common.mk
