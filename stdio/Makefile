CFLAGS		+= -std=c99 -pedantic -Wshadow -I../include -I./output -I./format -I./input
DEPFLAGS	?= -MMD -MP
NAME_LIB	=		libctype
NAME		=		$(NAME_LIB).a
LIB_DEPS	?=
DIR_EXCLUDE	?= input
PROVIDES	?=	ctype
SRCDIR := .
# Exclude subdirectories from recursive source search (relative to SRCDIR)
DIR_EXCLUDE ?= input

OBJ_DIR		?=	obj

# collect all .c files under the ctype tree
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c')))

OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)
#include only existing dependency files to avoid make errors when obj/ or test .d files are absent */
include $(wildcard $(DPDC))
# don't compute SRCS here — let common.mk compute SRCS using DIR_EXCLUDE/FIND_SRCS
# OBJS/DPDC are derived by common.mk after SRCS is known.

# include common rules after variables so common.mk uses our SRCS/OBJS/DPDC
include ../mk/common.mk
