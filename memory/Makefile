# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/30 19:03:05 by                   #+#    #+#              #
#    Updated: 2025/11/30 19:03:52 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# project-specific settings (variables kept overrideable)
CFLAGS += -std=c99 -pedantic -Wshadow -I../include -I../memory -I./ft_printf/include
DEPFLAGS ?= -MMD -MP

NAME_LIB	=		libmemory
NAME		=		$(NAME_LIB).a

# This library provides the "memory" library name for consumers.
# Consumers can depend on "memory" using TEST_DEPS or DEPS.
PROVIDES ?= memory

# Optional: list dependencies (short names, libNAME, or paths). Example:
# TEST_DEPS ?= string stdio
# or to use specific archives by path:
# TEST_DEPS ?= ../strings/build/libstring.a
TEST_DEPS ?=

SRCDIR := .
SRCS := $(sort $(patsubst ./%,%,$(shell find $(SRCDIR) -type f -name '*.c')))

OBJ_DIR		?=	obj

OBJS		:=	$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
DPDC		:=	$(OBJS:.o=.d)

# include dependency files if present
-include $(DPDC)

# include common rules after variables so common.mk uses our SRCS/OBJS/DPDC
include ../mk/common.mk