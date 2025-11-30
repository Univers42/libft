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
# Exclude subdirectories from recursive source search (relative to SRCDIR)
DIR_EXCLUDE := input

OBJ_DIR		?=	obj

# let common.mk compute SRCS/OBJS/DPDC after it processes SRCDIR and DIR_EXCLUDE
# (use Makefile-style comment; C-style /* ... */ breaks make)
include ../mk/common.mk