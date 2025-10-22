# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    debug.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/27 19:30:00 by dlesieur          #+#    #+#              #
#    Updated: 2025/10/22 22:31:30 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ════════════════════════════════════════════════════════════════════════════
# ║                            DEBUG TARGETS                                ║
# ════════════════════════════════════════════════════════════════════════════

THIS_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
include $(THIS_DIR)common.mk

# Debug flags
DEBUG_FLAGS = -g3 -DDEBUG
SANITIZE_FLAGS = -fsanitize=address -fsanitize=undefined
OPTIMIZE_FLAGS = -O0

# Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS) $(OPTIMIZE_FLAGS)
debug: fclean all
	$(call print_status,$(YELLOW),DEBUG,Debug mode enabled)

# Debug with sanitizers
.PHONY: debug_sanitize
debug_sanitize: CFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS) $(OPTIMIZE_FLAGS)
debug_sanitize: fclean all
	$(call print_status,$(YELLOW),DEBUG,Debug mode with sanitizers enabled)
# $(call logging,STATE_TOKEN,Message)
# Produces: configure [STATE] : Message (colorized)
define logging
	printf "${BRIGHT_CYAN}${BOLD}%s${RESET} [${BOLD}%b${RESET}] : %b\n" "$(LOG_PREFIX)" "$(1)" "$(2)"
endef

define log_info
	$(call logging,$(STATE_INFO),$(1))
endef

define log_warn
	$(call logging,$(STATE_WARN),$(1))
endef

define log_ok
	$(call logging,$(STATE_OK),$(1))
endef

define log_note
	$(call logging,$(STATE_NOTE),$(1))
endef

define log_error
	$(call logging,$(STATE_ERROR),$(1))
endef

define log_debug
	$(call logging,$(STATE_DEBUG),$(1))
endef

# Release build
.PHONY: release
release: CFLAGS += -O3 -DNDEBUG
release: fclean all
	$(call print_status,$(GREEN),RELEASE,Release mode enabled)

# Profile build
.PHONY: profile
profile: CFLAGS += -pg -O2
profile: fclean all
	$(call print_status,$(BLUE),PROFILE,Profile mode enabled)

# Static analysis
.PHONY: analyze
analyze:
	$(call print_status,$(CYAN),ANALYZE,Running static analysis...)
	@cppcheck --enable=all --std=c99 $(SRC_DIRS) 2>/dev/null || true
	@scan-build make fclean all 2>/dev/null || true
