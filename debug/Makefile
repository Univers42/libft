CFLAGS		+=	-std=c99 -pedantic -Wshadow -I../include -I.
DEPFLAGS	?=	-MMD -MP
NAME_LIB	=	libdebug
NAME		=	$(NAME_LIB).a
LIB_DEPS	?=	
PROVIDES	?=
SRCDIR		:=	.
SRCS		:=	$(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name "*.c")))
OBJ_DIR		?=	obj
OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)

-include $(DPDC)
include ../mk/common.mk