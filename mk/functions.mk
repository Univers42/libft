# Function to create directory structure
define create_dirs
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	# NOTE: do not create $(BUILD_DIR) automatically to avoid polluting the repo
	# If you really need build/ for tools, create it manually or via an explicit target.
endef

# Function to print status messages (single-line to avoid breaking shell blocks)
define print_status
	printf '%s%s%s[%s%s%s]%s: %s%s\n' "$(FADED_BOLD_GRAY)$(notdir $(CURDIR))$(FADED_BOLD_GRAY)" "$(1)" "$(2)" "$(FADED_BOLD_GRAY)$(FADED_BOLD_GRAY)$(WHITE)$(3)$(RESET)"
endef

# General logging macro (single-line)
define logging
	printf '%s%s%s[%s%s%s]%s: %s%s\n' "$(FADED_BOLD_GRAY)$(notdir $(CURDIR))$(FADED_BOLD_GRAY)" "$(1)" "$(2)" "$(FADED_BOLD_GRAY)$(FADED_BOLD_GRAY)$(WHITE)$(3)$(RESET)"
endef

# Function to clean object files (use rm -rf to remove directory reliably)
define clean_objects
	@$(call print_status,$(RED),CLEAN,Removing object files...)
	@rm -rf $(OBJ_DIR)
endef

# Function to clean library
define clean_library
	@$(call print_status,$(RED),CLEAN,Removing library $(NAME)...)
	@$(RM) $(NAME)
endef

define log_info
	$(call logging,$(CYAN),INFO,$(1))
endef

define log_warn
	$(call logging,$(YELLOW),WARN,$(1))
endef

define log_ok
	$(call logging,$(GREEN),OK,$(1))
endef

define log_note
	$(call logging,$(BLUE),NOTE,$(1))
endef

define log_error
	$(call logging,$(RED),ERROR,$(1))
endef

define log_debug
	$(call logging,$(MAGENTA),DEBUG,$(1))
endef