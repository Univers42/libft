# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    colors.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/27 19:30:00 by dlesieur          #+#    #+#              #
#    Updated: 2025/06/11 15:42:52 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ════════════════════════════════════════════════════════════════════════════
# ║                            COLOR DEFINITIONS                             ║
# ════════════════════════════════════════════════════════════════════════════

# Basic colors
BLACK        := $(shell printf '\033[0;30m')
RED          := $(shell printf '\033[0;31m')
GREEN        := $(shell printf '\033[0;32m')
YELLOW       := $(shell printf '\033[0;33m')
BLUE         := $(shell printf '\033[0;34m')
MAGENTA      := $(shell printf '\033[0;35m')
CYAN         := $(shell printf '\033[0;36m')
WHITE        := $(shell printf '\033[0;37m')

# Bold colors
BOLD_BLACK   := $(shell printf '\033[1;30m')
BOLD_RED     := $(shell printf '\033[1;31m')
BOLD_GREEN   := $(shell printf '\033[1;32m')
BOLD_YELLOW  := $(shell printf '\033[1;33m')
BOLD_BLUE    := $(shell printf '\033[1;34m')
BOLD_MAGENTA := $(shell printf '\033[1;35m')
BOLD_CYAN    := $(shell printf '\033[1;36m')
BOLD_WHITE   := $(shell printf '\033[1;37m')

# Background colors
BG_BLACK     := $(shell printf '\033[40m')
BG_RED       := $(shell printf '\033[41m')
BG_GREEN     := $(shell printf '\033[42m')
BG_YELLOW    := $(shell printf '\033[43m')
BG_BLUE      := $(shell printf '\033[44m')
BG_MAGENTA   := $(shell printf '\033[45m')
BG_CYAN      := $(shell printf '\033[46m')
BG_WHITE     := $(shell printf '\033[47m')

# Special formatting
RESET        := $(shell printf '\033[0m')
BOLD         := $(shell printf '\033[1m')
DIM          := $(shell printf '\033[2m')
ITALIC       := $(shell printf '\033[3m')
UNDERLINE    := $(shell printf '\033[4m')
BLINK        := $(shell printf '\033[5m')
REVERSE      := $(shell printf '\033[7m')
STRIKETHROUGH:= $(shell printf '\033[9m')

# Enhanced colors and formatting
BRIGHT_GREEN := $(shell printf '\033[1;32m')
BRIGHT_RED   := $(shell printf '\033[1;31m')
BRIGHT_YELLOW:= $(shell printf '\033[1;33m')
BRIGHT_BLUE  := $(shell printf '\033[1;34m')
BRIGHT_MAGENTA:= $(shell printf '\033[1;35m')
BRIGHT_CYAN  := $(shell printf '\033[1;36m')

# State-specific colors (used by logging macro)
STATE_COLOR_INFO  := $(BRIGHT_CYAN)
STATE_COLOR_WARN  := $(BRIGHT_YELLOW)
STATE_COLOR_OK    := $(BRIGHT_GREEN)
STATE_COLOR_NOTE  := $(BRIGHT_MAGENTA)
STATE_COLOR_ERROR := $(BRIGHT_RED)
STATE_COLOR_DEBUG := $(BRIGHT_BLUE)