# Add the BONUS_BUILD variable at the top
NAME					=	libftprintf.a
BONUS_BUILD				=	0
CC						=	gcc
CFLAGS 					= 	-Wall -Werror -Wextra -g3 -O0 -fno-omit-frame-pointer -Wpedantic -Wshadow -Wundef -Wformat=2 -Wcast-align
# Option 2: Thread Sanitizer (for threading issues)
# CFLAGS = -Wall -Werror -Wextra -g3 -O0 -fsanitize=thread -fno-omit-frame-pointer \
#          -Wpedantic -Wshadow -Wundef -Wformat=2 -Wcast-align
AR						=	ar
ARFLAGS 				=	rcs
RM						=	rm -rf

# Directories
SRC_DIR					=	src
SRC_BONUS_DIR			=	src_bonus
INCLUDE_DIR				=	include
OBJ_DIR					=	obj
OBJ_BONUS_DIR			=	obj_bonus
BUILD_DIR				=	build
LIBFT_DIR				=	libft
JUMP_TABLE_UTILS_DIR	=	$(SRC_BONUS_DIR)/utils_jump_table
CORE_DIR				=	$(SRC_BONUS_DIR)/core
FORMAT_SPECIFIER_DIR	=	$(SRC_BONUS_DIR)/format_specifier
HELPERS_DIR 			=	$(SRC_BONUS_DIR)/helpers
PRECISION_DIR			=	$(SRC_BONUS_DIR)/precision
WIDTH_DIR				=	$(SRC_BONUS_DIR)/width
JUMP_TABLE_DIR			= 	$(SRC_BONUS_DIR)/jump_table

PROGRAM = program
# Terminal Colors
define COLORS
	# Text Colors
	BLACK       = \033[0;30m
	RED         = \033[0;31m
	GREEN       = \033[0;32m
	YELLOW      = \033[0;33m
	BLUE        = \033[0;34m
	MAGENTA     = \033[0;35m
	CYAN        = \033[0;36m
	WHITE       = \033[0;37m

	# Bold Text
	BBLACK      = \033[1;30m
	BRED        = \033[1;31m
	BGREEN      = \033[1;32m
	BYELLOW     = \033[1;33m
	BBLUE       = \033[1;34m
	BMAGENTA    = \033[1;35m
	BCYAN       = \033[1;36m
	BWHITE      = \033[1;37m

	# Background Colors
	BG_BLACK    = \033[40m
	BG_RED      = \033[41m
	BG_GREEN    = \033[42m
	BG_YELLOW   = \033[43m
	BG_BLUE     = \033[44m
	BG_MAGENTA  = \033[45m
	BG_CYAN     = \033[46m
	BG_WHITE    = \033[47m

	# Special
	RESET       = \033[0m
	BOLD        = \033[1m
	UNDERLINE   = \033[4m
	BLINK       = \033[5m
	REVERSE     = \033[7m
	HIDDEN      = \033[8m

endef

$(eval $(COLORS))

# ============================================================================ #
# =====                          SOURCE FILES                             ===== #
# ============================================================================ #

# Standard implementation source files
SRCS = $(SRC_DIR)/ft_format.c $(SRC_DIR)/ft_printf.c

# Generate object files with preserved directory structure
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Bonus implementation source files
SRCS_BONUS = 	$(JUMP_TABLE_UTILS_DIR)/ft_convert_wrappers_bonus.c $(JUMP_TABLE_UTILS_DIR)/ft_convert_wrappers_two_bonus.c \
				$(JUMP_TABLE_UTILS_DIR)/ft_flag_wrappers_bonus.c $(JUMP_TABLE_UTILS_DIR)/ft_flag_wrappers_two_bonus.c \
				$(JUMP_TABLE_DIR)/ft_init_bonus.c $(JUMP_TABLE_DIR)/ft_jump_flags_bonus.c \
				$(CORE_DIR)/ft_flags_init_bonus.c $(CORE_DIR)/ft_parser_bonus.c $(CORE_DIR)/ft_printf_bonus.c\
				$(FORMAT_SPECIFIER_DIR)/ft_char_bonus.c $(FORMAT_SPECIFIER_DIR)/ft_hex_bonus.c $(FORMAT_SPECIFIER_DIR)/ft_int_bonus.c \
				$(FORMAT_SPECIFIER_DIR)/ft_percent_bonus.c $(FORMAT_SPECIFIER_DIR)/ft_ptr_bonus.c $(FORMAT_SPECIFIER_DIR)/ft_ptr_bonus.c \
				$(FORMAT_SPECIFIER_DIR)/ft_str_bonus.c $(FORMAT_SPECIFIER_DIR)/ft_unsigned_bonus.c \
				$(WIDTH_DIR)/ft_width_core_bonus.c $(WIDTH_DIR)/ft_width_hex_bonus.c $(WIDTH_DIR)/ft_width_int_bonus.c \
				$(WIDTH_DIR)/ft_width_ptr_extra_bonus.c $(WIDTH_DIR)/ft_width_ptr_bonus.c \
				$(PRECISION_DIR)/ft_precision_core_bonus.c $(PRECISION_DIR)/ft_precision_hex_bonus.c $(PRECISION_DIR)/ft_precision_int_bonus.c \
				$(HELPERS_DIR)/ft_utils_int_bonus.c $(HELPERS_DIR)/ft_utils_ptr_bonus.c\
				$(HELPERS_DIR)/ft_flag_instructions_bonus.c $(HELPERS_DIR)/ft_generic_utils_bonus.c $(HELPERS_DIR)/ft_utils_format_bonus.c

# Generate bonus object files
OBJS_BONUS = $(patsubst $(SRC_BONUS_DIR)/%.c, $(OBJ_BONUS_DIR)/%.o, $(SRCS_BONUS))

# Create lists of subdirectories for object files
OBJ_SUBDIRS = $(sort $(dir $(OBJS)))
OBJ_BONUS_SUBDIRS = $(sort $(dir $(OBJS_BONUS)))

# Libft Library
LIBFT = $(LIBFT_DIR)/libft.a

# ============================================================================ #
# =====                         TARGETS & RULES                           ===== #
# ============================================================================ #

# ----- Default Target ----- #
all: $(NAME)

# ----- Bonus Target ----- #
bonus:
	@$(MAKE) BONUS_BUILD=1 $(NAME)

# ----- Library Creation based on build type ----- #
ifeq ($(BONUS_BUILD), 1)
# Bonus version
$(NAME): $(LIBFT) | $(OBJS_BONUS)
	@printf "\n$(BBLUE)╔══════════════════════════════════════════════════════════════════╗$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BMAGENTA)Creating bonus version into $(NAME)...$(RESET)                    $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)╚══════════════════════════════════════════════════════════════════╝$(RESET)\n"
	@cp $(LIBFT) $(NAME)
	@$(AR) $(ARFLAGS) $(NAME) $(OBJS_BONUS) > /dev/null
	@printf "$(BGREEN)$(BOLD)✓ $(NAME) with bonus features created successfully$(RESET)\n"
else

# Standard version
$(NAME): $(OBJS) | $(LIBFT)
	@printf "\n$(BBLUE)╔══════════════════════════════════════════════════════════════════╗$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BMAGENTA)Creating $(NAME)...$(RESET)                                        $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)╚══════════════════════════════════════════════════════════════════╝$(RESET)\n"
	@cp $(LIBFT) $(NAME)
	@$(AR) $(ARFLAGS) $(NAME) $(OBJS) > /dev/null
	@printf "$(BGREEN)$(BOLD)✓ $(NAME) created successfully$(RESET)\n"
endif

# ----- Run Target ----- #
run: all
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) -I $(INCLUDE_DIR) main.c $(NAME) -o $(BUILD_DIR)/outDebug
	@printf "$(BGREEN)Executable created at $(UNDERLINE)$(BUILD_DIR)/outDebug$(RESET)\n"

compile_main: all
	@if [ -f main.c ]; then \
		printf "Handled gracefully if main.c exists\n"; \
		printf "Now we compile the program.\n"; \
		$(CC) $(CFLAGS) main.c -I $(INCLUDE_DIR) -L. -lftprintf -o program; \
	else \
		printf "I didn't encounter any 'main.c', make sure of the name.\n"; \
	fi

run_bonus:
	@$(MAKE) BONUS_BUILD=1 --no-print-directory
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) -I $(INCLUDE_DIR) main.c $(NAME) -o $(BUILD_DIR)/outDebugBonus
	@printf "$(BGREEN)Bonus executable created at $(UNDERLINE)$(BUILD_DIR)/outDebugBonus$(RESET)\n"

# ----- Create Object Directories ----- #
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_SUBDIRS)
	@printf "$(BCYAN)Compiling: $(RESET) $<\n"
	@$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@ > /dev/null

$(OBJ_SUBDIRS):
	@mkdir -p $@

# ----- Create Bonus Object Directories and Files ----- #
$(OBJ_BONUS_DIR)/%.o: $(SRC_BONUS_DIR)/%.c | $(OBJ_BONUS_SUBDIRS)
	@printf "$(BCYAN)Compiling (bonus): $(RESET) $<\n"
	@$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@ > /dev/null

$(OBJ_BONUS_SUBDIRS):
	@mkdir -p $@

# ----- Build Libft ----- #
$(LIBFT):
	@printf "\n$(BMAGENTA)╔══════════════════════════════════════════════════════════════════╗$(RESET)\n"
	@printf "$(BMAGENTA)║$(RESET) Building libft...                                       $(BMAGENTA)         ║$(RESET)\n"
	@printf "$(BMAGENTA)╚══════════════════════════════════════════════════════════════════╝$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) CFLAGS="$(CFLAGS)" --no-print-directory > /dev/null
	@printf "$(BGREEN)✓ libft compiled$(RESET)\n"

# ----- Clean Object Files ----- #
clean:
	@printf "\n$(BG_YELLOW)$(BLACK)  CLEANING  $(RESET) "
	@printf "$(YELLOW)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n\n"
	
	@printf "$(YELLOW)╔═══════════════════════════════════════════════════════════════╗$(RESET)\n"
	@printf "$(YELLOW)║  🧹 $(BOLD)Removing object files...$(RESET)                                  $(YELLOW)║$(RESET)\n"
	@printf "$(YELLOW)╠═══════════════════════════════════════════════════════════════╣$(RESET)\n"
	@printf "$(YELLOW)║  ↪ Cleaning standard objects                                  $(YELLOW)║$(RESET)\n"
	@$(RM) $(OBJ_DIR) > /dev/null 2>&1 || true
	@printf "$(YELLOW)║  ↪ Cleaning bonus objects                                     $(YELLOW)║$(RESET)\n"
	@$(RM) $(OBJ_BONUS_DIR) > /dev/null 2>&1 || true
	@printf "$(YELLOW)║  ↪ Cleaning libft objects                                     $(YELLOW)║$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1 || true
	@printf "$(YELLOW)╚═══════════════════════════════════════════════════════════════╝$(RESET)\n\n"
	@printf "$(BGREEN)✓ Object files cleanup complete!$(RESET)\n\n"

# ----- Clean Everything ----- #
fclean: 
	@printf "\n$(BG_RED)$(WHITE)  FULL CLEANUP  $(RESET) "
	@printf "$(RED)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n\n"
	
	@printf "$(RED)╔═══════════════════════════════════════════════════════════════╗$(RESET)\n"
	@printf "$(RED)║  🔥 $(BOLD)Full cleanup in progress...$(RESET)                               $(RED)║$(RESET)\n"
	@printf "$(RED)╠═══════════════════════════════════════════════════════════════╣$(RESET)\n"
	@printf "$(RED)║  ↪ Removing standard objects                                  $(RED)║$(RESET)\n"
	@$(RM) $(OBJ_DIR) > /dev/null 2>&1 || true
	@printf "$(RED)║  ↪ Removing bonus objects                                     $(RED)║$(RESET)\n"
	@$(RM) $(OBJ_BONUS_DIR) > /dev/null 2>&1 || true
	@printf "$(RED)║  ↪ Removing $(NAME)                                     $(RED)║$(RESET)\n"
	@$(RM) $(NAME) > /dev/null 2>&1 || true
	@printf "$(RED)║  ↪ Removing executables                                       $(RED)║$(RESET)\n"
	@$(RM) $(BUILD_DIR) $(PROGRAM) > /dev/null 2>&1 || true
	@printf "$(RED)║  ↪ Executing libft fclean                                     $(RED)║$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1 || true
	@printf "$(RED)╚═══════════════════════════════════════════════════════════════╝$(RESET)\n\n"
	@printf "$(BGREEN)✓ Full cleanup complete! All generated files removed.$(RESET)\n\n"

# ----- Rebuild ----- #
re: fclean all

re_bonus: fclean 
	@$(MAKE) BONUS_BUILD=1 --no-print-directory

# ----- Help ----- #
help:
	@printf "\n$(BBLUE)╔═════════════════════════════════════════════════════════════╗$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BMAGENTA)ft_printf Library - Available Commands$(RESET)                      $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)╠═════════════════════════════════════════════════════════════╣$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make$(RESET)           - Build the mandatory library ($(NAME))$(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make bonus$(RESET)     - Build the bonus library ($(NAME))$(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make run$(RESET)       - Build and run with mandatory version       $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make run_bonus$(RESET) - Build and run with bonus version           $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make clean$(RESET)     - Remove object files                        $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make fclean$(RESET)    - Remove all generated files                 $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make re$(RESET)        - Rebuild mandatory version                  $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make re_bonus$(RESET)  - Rebuild bonus version                      $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)║$(RESET) $(BGREEN)make help$(RESET)      - Display this help message                  $(BBLUE)║$(RESET)\n"
	@printf "$(BBLUE)╚═════════════════════════════════════════════════════════════╝$(RESET)\n\n"

# ----- PHONY Targets ----- #
.PHONY: all clean fclean re bonus re_bonus run run_bonus help