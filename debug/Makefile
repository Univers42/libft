DEPFLAGS	?=	-MMD -MP
NAME_LIB	=	libdebug
NAME		=	$(NAME_LIB).a
LIB_DEPS	=	stdio std 
PROVIDES	?= debug
SRCDIR		:=	.
SRCS		:=	$(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name "*.c")))
OBJ_DIR		?=	obj
OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)

# Ensure tests build and link declared library dependencies.
TEST_DEPS	:=	$(LIB_DEPS)

# Convenience: enable NORMA abort mode via `make NORMA=1`
NORMA		?=	0
EXTRA_DEFINES ?=
# Always enable DEBUG for this module so backtrace is printed.
USER_DEFINES	+= -DDEBUG $(EXTRA_DEFINES)
ifeq ($(NORMA),1)
	USER_DEFINES	+= -DNORMA
endif

-include $(DPDC)
include ../mk/common.mk