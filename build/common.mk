# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    common.mk                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/27 19:30:00 by dlesieur          #+#    #+#              #
#    Updated: 2025/05/27 19:10:29 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ════════════════════════════════════════════════════════════════════════════
# ║                           COMMON VARIABLES                               ║
# ════════════════════════════════════════════════════════════════════════════

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -f

# Directories
OBJ_DIR = obj
BUILD_DIR = build

# Colors and formatting
include $(BUILD_DIR)/colors.mk

# ════════════════════════════════════════════════════════════════════════════
# ║                           COMMON FUNCTIONS                               ║
# ════════════════════════════════════════════════════════════════════════════

# Function to create directory structure
define create_dirs
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BUILD_DIR)
endef

# Function to print compilation progress
define print_progress
	@$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
	@printf "\r$(YELLOW)Compiling: [%-30s] %d%%$(RESET)" \
		"$(shell printf '%*s' $$(($(COUNTER) * 30 / $(TOTAL_FILES))) | tr ' ' '▓')" \
		"$$(($(COUNTER) * 100 / $(TOTAL_FILES)))"
endef

# Function to print status messages
define print_status
	@printf "$(1)$(BOLD)[$(2)] $(3)$(RESET)\n"
endef

# Function to clean object files
define clean_objects
	$(call print_status,$(RED),CLEAN,Removing object files...)
	@$(RM) -r $(OBJ_DIR)
endef

# Function to clean library
define clean_library
	$(call print_status,$(RED),CLEAN,Removing library $(NAME)...)
	@$(RM) $(NAME)
endef
