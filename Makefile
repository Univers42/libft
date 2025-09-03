# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/26 12:30:42 by dlesieur          #+#    #+#              #
#    Updated: 2025/09/03 16:04:00 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Static MAKEFILE includes
include build/colors.mk
include build/common.mk

define build_lib
	$(MAKE) -C $(1) $(2)
endef

define trans_remove_c
	find . -type f -name "main.c" -exec sh -c 'mv "$$1" "$${1%.c}.bak"' _ {} \;
endef

define trans_add_c
	find . -type f -name "main.bak" -exec sh -c 'mv "$$1" "$${1%.bak}.c"' _ {} \;
endef

# MAIN ?=

# Library name
NAME = libft.a
MINILIBX_DIR ?= minilibx-linux
MINILIBX_LIB ?= $(MINILIBX_DIR)/libmlx.a
MLX_ALT_LIB ?= $(MINILIBX_DIR)/mlx.a
MLX_FLAGS ?=

# Detect availability of MiniLibX (directory + makefile)
MLX_ENABLED := 0
ifneq ($(wildcard $(MINILIBX_DIR)),)
ifneq ($(or $(wildcard $(MINILIBX_DIR)/Makefile),$(wildcard $(MINILIBX_DIR)/makefile)),)
MLX_ENABLED := 1
endif
endif

# Tag directories that depend on MiniLibX
MLX_TAG_DIRS ?= classes/render/window classes/render/inputHandler classes/render/event

# Allow users to force-skip directories externally
SKIP_DIRS ?=
ifeq ($(MLX_ENABLED),0)
SKIP_DIRS += $(MLX_TAG_DIRS)
endif

# Directories
DATA_STRUCTURES = 	data_structures/queue data_structures/circular_linked_list data_structures/doubly_linked_list\
					data_structures/lists data_structures/vector classes/render/map classes/render/window classes/render/inputHandler classes/render/point classes/render/line classes/trace/error \
					stdlib/arena stdlib/pool stdlib/slab classes/render/event \
					math/geometry math/geometry/draw math/geometry/effects math/geometry/matrices/ math/geometry/move math/geometry/quaternion \
					math/geometry/shapes math/geometry/view math/algebra

# classes classes/render classes/render/window classes/render/inputHandler classes/render/point/ classes/render/camera
SRC_DIRS = ctype debug $(DATA_STRUCTURES) math memory time render stdio stdio/ft_printf/src stdio/ft_scanf stdio/ft_scanf/parse_format stdlib strings sort classes/render/inputHandler

# Compute effective source directories after excluding tagged ones
SRC_DIRS_EFF := $(filter-out $(SKIP_DIRS),$(SRC_DIRS))

# Source files from all directories including ft_printf and gnl
SRCS = $(foreach dir,$(SRC_DIRS_EFF),$(wildcard $(dir)/*.c))
PRINTF_SRCS = $(shell find stdio/ft_printf/src -name "*.c" 2>/dev/null)
GNL_SRCS = $(wildcard stdio/gnl/*.c)
ALL_SRCS = $(SRCS) $(PRINTF_SRCS) $(GNL_SRCS)
# Exclude every main.c from the library to avoid multiple definition
ALL_SRCS := $(filter-out %/main.c,$(ALL_SRCS))

# Object files with directory structure preserved
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))

# Header files
HEADERS = $(wildcard *.h)

# Find all unique directories containing .h files in libft, always include root dir
HEADER_DIRS := $(shell find . -type f -name '*.h' -exec dirname {} \; | sort -u)
HEADER_DIRS += .
INCLUDE_FLAGS := $(foreach dir,$(HEADER_DIRS),-I$(dir))

# Add INCLUDE_FLAGS to CFLAGS
CFLAGS += $(INCLUDE_FLAGS) -g3

# Progress/animation settings
PROGRESS_WIDTH ?= 30
COUNTER_FILE := $(OBJ_DIR)/.build_count
SPIN_FILE := $(OBJ_DIR)/.spin_state
TOTAL_OBJS := $(words $(OBJS))

# Default rule: build the library
ifeq ($(MLX_ENABLED),1)
all: $(MINILIBX_LIB) $(NAME)
else
all: $(NAME)
endif

# Build MiniLibX only if present; otherwise skip gracefully
$(MINILIBX_LIB):
	@echo "$(BRIGHT_CYAN)🔧 Building MiniLibX...$(RESET)"
	@if [ -d "$(MINILIBX_DIR)" ] && { [ -f "$(MINILIBX_DIR)/Makefile" ] || [ -f "$(MINILIBX_DIR)/makefile" ]; }; then \
		$(MAKE) -C $(MINILIBX_DIR) $(MLX_FLAGS) || true; \
		if [ ! -f "$(MINILIBX_LIB)" ] && [ -f "$(MLX_ALT_LIB)" ]; then \
			cp "$(MLX_ALT_LIB)" "$(MINILIBX_LIB)"; \
			echo "$(YELLOW)[MiniLibX] Fallback: copied mlx.a -> libmlx.a$(RESET)"; \
		fi; \
	else \
		echo "$(YELLOW)[MiniLibX] Skipping (not present)$(RESET)"; \
	fi

# Prepare progress state before building objects
.PHONY: progress_init
progress_init:
	@mkdir -p $(OBJ_DIR)
	@rm -f $(COUNTER_FILE) $(SPIN_FILE)
	@echo 0 > $(COUNTER_FILE)
	@echo 0 > $(SPIN_FILE)
	@printf "$(BRIGHT_MAGENTA)🚀 Starting build of $(NAME) ($(TOTAL_OBJS) objects)$(RESET)\n"

# Rule to build the library
$(NAME): progress_init $(OBJS)
	@{ \
		# Finalize progress to 100% (visual polish) \
		count=$$( [ -f "$(COUNTER_FILE)" ] && cat "$(COUNTER_FILE)" || echo 0 ); \
		total=$(TOTAL_OBJS); \
		width=$(PROGRESS_WIDTH); \
		percent=100; \
		done_chars=$$width; \
		bar_done=$$(printf "%0.s#" $$(seq 1 $$done_chars)); \
		# Show a final full bar line before archiving \
		printf "\r$(BRIGHT_CYAN)[✔]$(RESET) $(DIM)building$(RESET) $(YELLOW)%-30s$(RESET) $(BRIGHT_GREEN)[%s] %3d%%$(RESET) (%d/%d)\n" \
			"finalizing" "$$bar_done" "$$percent" "$$total" "$$total"; \
	}
	@printf "$(BRIGHT_MAGENTA)📦 Archiving library...$(RESET)\n"
	@$(AR) $(NAME) $(OBJS)
	@printf "$(BRIGHT_GREEN)✓ $(NAME) created successfully!$(RESET)\n"

# Object compilation rule with live spinner and inline progress bar
$(OBJ_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	@{ \
		# Start a spinner while compiling this file \
		i=0; \
		spinner() { \
			i=0; \
			while :; do \
				case $$((i % 4)) in \
					0) sym='|';; 1) sym='/';; 2) sym='-';; 3) sym='\\';; \
				esac; \
				printf "\r$(BRIGHT_CYAN)[%s]$(RESET) $(DIM)compiling$(RESET) $(YELLOW)%-30s$(RESET)" "$$sym" "$(notdir $<)"; \
				i=$$((i + 1)); \
				sleep 0.1; \
			done; \
		}; \
		spinner & SPIN_PID=$$!; \
		# Compile \
		$(CC) $(CFLAGS) -c $< -o $@; STATUS=$$?; \
		# Stop spinner \
		kill -9 $$SPIN_PID >/dev/null 2>&1 || true; \
		wait $$SPIN_PID 2>/dev/null || true; \
		# Progress accounting \
		count=0; [ -f "$(COUNTER_FILE)" ] && count=$$(cat "$(COUNTER_FILE)"); \
		count=$$((count + 1)); echo $$count > "$(COUNTER_FILE)"; \
		total=$(TOTAL_OBJS); \
		[ $$total -eq 0 ] && total=1; \
		percent=$$((100 * count / total)); \
		# Clamp percent to [0..100] visually \
		if [ $$percent -gt 100 ]; then percent=100; fi; \
		if [ $$percent -lt 0 ]; then percent=0; fi; \
		width=$(PROGRESS_WIDTH); \
		done_chars=$$((percent * width / 100)); \
		[ $$done_chars -gt $$width ] && done_chars=$$width; \
		bar_done=$$(printf "%0.s#" $$(seq 1 $$done_chars)); \
		bar_pad=$$(printf "%0.s " $$(seq 1 $$((width - done_chars)))); \
		if [ $$STATUS -eq 0 ]; then \
			printf "\r$(BRIGHT_CYAN)[✔]$(RESET) $(DIM)building$(RESET) $(YELLOW)%-30s$(RESET) $(BRIGHT_GREEN)[%s%s] %3d%%$(RESET) (%d/%d)" \
				"$(notdir $<)" "$$bar_done" "$$bar_pad" "$$percent" "$$count" "$$total"; \
		else \
			printf "\r$(BRIGHT_RED)[✗]$(RESET) $(YELLOW)%-30s$(RESET) $(BRIGHT_RED)FAILED$(RESET)\n" "$(notdir $<)"; \
			exit $$STATUS; \
		fi; \
	}

build :
	@call(build_lib($(MINILIBX), all));
	
# Enhanced clean with visual feedback
clean:
	@echo "$(BRIGHT_RED)🧹 Cleaning object files...$(RESET)"
	@if [ -d "$(OBJ_DIR)" ]; then \
		echo "$(YELLOW)Removing $(OBJ_DIR)/ directory...$(RESET)"; \
		$(RM) -r $(OBJ_DIR); \
		echo "$(GREEN)✅ Object files cleaned$(RESET)"; \
	else \
		echo "$(DIM)No object files to clean$(RESET)"; \
	fi
	@rm -f $(COUNTER_FILE) $(SPIN_FILE)
	@echo "$(BRIGHT_RED)🧹 Cleaning MiniLibX...$(RESET)"
	@if [ -d "$(MINILIBX_DIR)" ] && { [ -f "$(MINILIBX_DIR)/Makefile" ] || [ -f "$(MINILIBX_DIR)/makefile" ]; }; then \
		$(MAKE) -C $(MINILIBX_DIR) clean || true; \
	else \
		echo "$(DIM)Skipping MiniLibX clean (not present)$(RESET)"; \
	fi
	@echo "$(BRIGHT_RED)🧹 Cleaning documentation...$(RESET)"
	@$(RM) -r docs

# Enhanced fclean with visual feedback
fclean: clean
	@echo "$(BRIGHT_RED)🔥 Deep cleaning...$(RESET)"
	@if [ -f "$(NAME)" ]; then \
		echo "$(YELLOW)Removing library $(NAME)...$(RESET)"; \
		$(RM) $(NAME); \
		echo "$(GREEN)✅ Library removed$(RESET)"; \
	else \
		echo "$(DIM)No library to remove$(RESET)"; \
	fi
	@echo "$(BRIGHT_RED)🔥 Deep cleaning MiniLibX...$(RESET)"
	@if [ -d "$(MINILIBX_DIR)" ] && { [ -f "$(MINILIBX_DIR)/Makefile" ] || [ -f "$(MINILIBX_DIR)/makefile" ]; }; then \
		$(MAKE) -C $(MINILIBX_DIR) clean || true; \
	else \
		echo "$(DIM)Skipping MiniLibX deep clean (not present)$(RESET)"; \
	fi
	@if [ -d "$(DOC_DIR)" ]; then \
		echo "$(YELLOW)Removing documentation directory...$(RESET)"; \
		$(RM) -r $(DOC_DIR); \
	fi

# Rebuild
re: fclean all

# Debug mode with enhanced feedback
debug: CFLAGS += -g3 -fsanitize=address
debug: 
	@echo "$(BRIGHT_YELLOW)🐛 DEBUG MODE ENABLED$(RESET)"
	@echo "$(YELLOW)Flags: $(CFLAGS)$(RESET)"
	@$(MAKE) re --no-print-directory
	@echo "$(BRIGHT_YELLOW)🔍 Debug build completed with AddressSanitizer$(RESET)"

# Build a test executable from a main.c in a subdirectory
# Usage: make test TEST=window
TEST ?=

ifeq ($(TEST),)
TEST_MAIN :=
else
TEST_MAIN := $(shell find . -type f -path "*/$(TEST)/main.c" | head -n 1)
endif

TEST_EXE := $(TEST)

.PHONY: test $(TEST_EXE)
mode_42:
	@$(trans_remove_c)
mode_lab:
	@$(trans_add_c)

test: $(TEST_EXE)

norminette:
	find . -path "./minilibx-linux" -prune -o -name "*.c" -exec norminette {} +

# Optional link flags for tests (only when MiniLibX is available)
ifeq ($(MLX_ENABLED),1)
LINK_MLX := $(MINILIBX_LIB) -lX11 -lXext -lm
else
LINK_MLX :=
endif

$(TEST_EXE): $(MINILIBX_LIB) $(NAME)
ifneq ($(strip $(TEST_MAIN)),)
	@mkdir -p $(dir ./bin/$@)
	$(CC) $(CFLAGS) -o ./bin/$@ $(TEST_MAIN) $(NAME) $(LINK_MLX)
	@printf "Built test executable: ./bin/%s\n" "$@"
else
	@echo "No main.c found for test '$(TEST)'"
	@exit 1
endif

# Documentation building
DOC_DIR = docs
DOC_GENERATOR = doc-generator
DOCS_OUTPUT = $(DOC_DIR)/dist

.PHONY: docs docs-dev docs-build docs-clean docs-serve docs-init docs-setup

# Build documentation
docs: docs-build

# Development mode with hot reload
docs-dev:
	@echo "$(BRIGHT_CYAN)📚 Starting documentation dev server...$(RESET)"
	@if [ ! -d "$(DOC_DIR)" ]; then $(MAKE) docs-init; fi
	@cd $(DOC_DIR) && cargo run --bin dev-server

# Build static documentation
docs-build:
	@echo "$(BRIGHT_CYAN)📖 Building documentation...$(RESET)"
	@if [ ! -d "$(DOC_DIR)" ]; then \
		echo "$(YELLOW)Initializing documentation generator...$(RESET)"; \
		$(MAKE) docs-init; \
	fi
	@if ! command -v cargo >/dev/null 2>&1; then \
		echo "$(BRIGHT_RED)Error: Rust/Cargo not found. Please install Rust first:$(RESET)"; \
		echo "$(YELLOW)curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh$(RESET)"; \
		echo "$(YELLOW)source ~/.cargo/env$(RESET)"; \
		exit 1; \
	fi
	@cd $(DOC_DIR) && cargo run --bin doc-generator -- --source ../
	@echo "$(BRIGHT_GREEN)✓ Documentation built in $(DOCS_OUTPUT)$(RESET)"

# Initialize documentation project
docs-init:
	@echo "$(BRIGHT_MAGENTA)🚀 Creating documentation system...$(RESET)"
	@mkdir -p $(DOC_DIR)
	@$(MAKE) docs-setup

# Serve documentation locally
docs-serve: docs-build
	@echo "$(BRIGHT_GREEN)🌐 Serving documentation at http://localhost:8080$(RESET)"
	@cd $(DOCS_OUTPUT) && python3 -m http.server 8080 2>/dev/null || \
		cd $(DOCS_OUTPUT) && python -m SimpleHTTPServer 8080

# Clean documentation
docs-clean:
	@echo "$(BRIGHT_RED)🧹 Cleaning documentation...$(RESET)"
	@if [ -d "$(DOC_DIR)" ]; then \
		cd $(DOC_DIR) && cargo clean 2>/dev/null || true; \
		$(RM) -r $(DOC_DIR)/dist; \
	fi

# Setup documentation infrastructure
docs-setup:
	@echo "$(BRIGHT_CYAN)Setting up Rust documentation generator...$(RESET)"
	@if [ ! -f "$(DOC_DIR)/Cargo.toml" ]; then \
		echo "$(YELLOW)Creating Cargo.toml...$(RESET)"; \
		$(MAKE) docs-create-cargo; \
	fi
	@if [ ! -d "$(DOC_DIR)/src" ]; then \
		echo "$(YELLOW)Creating source files...$(RESET)"; \
		$(MAKE) docs-create-sources; \
	fi
	@echo "$(BRIGHT_GREEN)✓ Documentation generator ready$(RESET)"

docs-create-cargo:
	@printf '[package]\n' > $(DOC_DIR)/Cargo.toml
	@printf 'name = "libft-docs"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'version = "0.1.0"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'edition = "2021"\n\n' >> $(DOC_DIR)/Cargo.toml
	@printf '[[bin]]\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'name = "doc-generator"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'path = "src/generator.rs"\n\n' >> $(DOC_DIR)/Cargo.toml
	@printf '[[bin]]\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'name = "dev-server"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'path = "src/dev_server.rs"\n\n' >> $(DOC_DIR)/Cargo.toml
	@printf '[dependencies]\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'serde = { version = "1.0", features = ["derive"] }\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'serde_json = "1.0"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'tokio = { version = "1.0", features = ["full"] }\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'axum = "0.7"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'tower = "0.4"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'tower-http = { version = "0.5", features = ["fs", "cors"] }\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'handlebars = "4.5"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'walkdir = "2.4"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'regex = "1.10"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'clap = { version = "4.4", features = ["derive"] }\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'notify = "6.1"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'anyhow = "1.0"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'markdown = "1.0"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'syntect = "5.1"\n' >> $(DOC_DIR)/Cargo.toml
	@printf 'include_dir = "0.7"\n' >> $(DOC_DIR)/Cargo.toml

docs-create-sources:
	@mkdir -p $(DOC_DIR)/src $(DOC_DIR)/templates $(DOC_DIR)/static
	@$(MAKE) docs-create-lib docs-create-parser docs-create-generator docs-create-server docs-create-templates

docs-create-lib:
	@printf 'use serde::{Deserialize, Serialize};\n' > $(DOC_DIR)/src/lib.rs
	@printf 'use std::collections::HashMap;\n\n' >> $(DOC_DIR)/src/lib.rs
	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/lib.rs
	@printf 'pub struct LibraryMetadata {\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub name: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub version: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub description: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub author: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub categories: Vec<String>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub functions: HashMap<String, FunctionMetadata>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '}\n\n' >> $(DOC_DIR)/src/lib.rs
	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/lib.rs
	@printf 'pub struct FunctionMetadata {\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub name: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub category: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub tags: Vec<String>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub prototype: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub description: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub parameters: Vec<Parameter>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub return_value: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub examples: Vec<Example>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub complexity: Option<String>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub notes: Vec<String>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub see_also: Vec<String>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '}\n\n' >> $(DOC_DIR)/src/lib.rs
	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/lib.rs
	@printf 'pub struct Parameter {\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub name: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub type_name: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub description: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '}\n\n' >> $(DOC_DIR)/src/lib.rs
	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/lib.rs
	@printf 'pub struct Example {\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub title: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub code: String,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '    pub output: Option<String>,\n' >> $(DOC_DIR)/src/lib.rs
	@printf '}\n\n' >> $(DOC_DIR)/src/lib.rs
	@printf 'pub mod parser;\n' >> $(DOC_DIR)/src/lib.rs
	@printf 'pub mod generator;\n' >> $(DOC_DIR)/src/lib.rs
	@printf 'pub mod templates;\n' >> $(DOC_DIR)/src/lib.rs

docs-create-parser:
	@cp docs/src/parser.rs $(DOC_DIR)/src/parser.rs 2>/dev/null || \
		echo "// Parser implementation will be copied from existing file" > $(DOC_DIR)/src/parser.rs

docs-create-generator:
	@printf 'use clap::Parser;\n' > $(DOC_DIR)/src/generator.rs
	@printf 'use std::fs;\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'use serde::{Deserialize, Serialize};\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'use std::collections::HashMap;\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'use regex::Regex;\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'use walkdir::WalkDir;\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'use std::path::Path;\n\n' >> $(DOC_DIR)/src/generator.rs

	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'pub struct LibraryMetadata { pub name:String, pub version:String, pub description:String, pub author:String, pub categories:Vec<String>, pub functions:HashMap<String, FunctionMetadata>, }\n' >> $(DOC_DIR)/src/generator.rs
	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'pub struct FunctionMetadata { pub name:String, pub category:String, pub tags:Vec<String>, pub prototype:String, pub description:String, pub parameters:Vec<Parameter>, pub return_value:String, pub examples:Vec<Example>, pub complexity:Option<String>, pub notes:Vec<String>, pub see_also:Vec<String>, }\n' >> $(DOC_DIR)/src/generator.rs
	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'pub struct Parameter { pub name:String, pub type_name:String, pub description:String }\n' >> $(DOC_DIR)/src/generator.rs
	@printf '#[derive(Debug, Serialize, Deserialize)]\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'pub struct Example { pub title:String, pub code:String, pub output:Option<String> }\n\n' >> $(DOC_DIR)/src/generator.rs

	@printf 'pub struct LibftParser { source_dir:String }\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'impl LibftParser { pub fn new(source_dir:String)->Self{Self{source_dir}} pub fn parse(&self)->anyhow::Result<LibraryMetadata>{ let mut functions=HashMap::new(); let categories=self.discover_categories()?; let mut file_count=0; for entry in WalkDir::new(&self.source_dir).follow_links(true).into_iter().filter_map(|e|e.ok()){ if entry.file_type().is_file(){ if let Some(ext)=entry.path().extension(){ if ext==\"c\" && !entry.path().to_string_lossy().contains(\"main.c\"){ file_count+=1; if let Ok(func_meta)=self.parse_c_file(entry.path()){ if let Some(meta)=func_meta{ functions.insert(meta.name.clone(), meta); } } } } } } println!(\"📊 Processed {} C files, found {} functions\", file_count, functions.len()); Ok(LibraryMetadata{ name:\"libft\".into(), version:\"1.0.0\".into(), description:\"42 School C Library - Extended standard library functions\".into(), author:\"dlesieur\".into(), categories, functions }) }\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'fn discover_categories(&self)->anyhow::Result<Vec<String>>{ let mut categories=Vec::new(); for entry in WalkDir::new(&self.source_dir).max_depth(3).into_iter().filter_map(|e|e.ok()){ if entry.file_type().is_dir(){ if let Some(dir_name)=entry.file_name().to_str(){ if !dir_name.starts_with('.') && !dir_name.starts_with(\"obj\") && !dir_name.starts_with(\"build\") && dir_name!=\"libft\" && dir_name!=\"src\" { categories.push(dir_name.to_string()); } } } } categories.extend([\"ctype\",\"debug\",\"memory\",\"math\",\"stdio\",\"strings\",\"stdlib\",\"time\",\"render\",\"data_structures\",\"sort\"].iter().map(|s|s.to_string())); categories.sort(); categories.dedup(); Ok(categories) }\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'fn parse_c_file(&self, path:&Path)->anyhow::Result<Option<FunctionMetadata>>{ let content=fs::read_to_string(path)?; if !content.contains(\"(\") || content.contains(\"main.c\"){ return Ok(None); } let filename=path.file_stem().and_then(|s|s.to_str()).unwrap_or(\"unknown\"); let category=self.extract_category_from_path(path); let prototype=self.extract_function_prototype(&content, filename)?; let metadata=FunctionMetadata{ name:filename.to_string(), category, tags:self.generate_tags(filename,&content), prototype, description:self.extract_description(&content), parameters:Vec::new(), return_value:\"Return value description not available.\".into(), examples:vec![Example{ title:format!(\"Basic usage of {}\", filename), code:format!(\"// Example usage of {}\\n// TODO: Add real example\", filename), output:None }], complexity:None, notes:Vec::new(), see_also:Vec::new() }; Ok(Some(metadata)) }\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'fn extract_category_from_path(&self, path:&Path)->String{ let p=path.to_string_lossy(); if p.contains(\"strings\"){\"strings\"} else if p.contains(\"memory\"){\"memory\"} else if p.contains(\"math\"){\"math\"} else if p.contains(\"stdio\"){\"stdio\"} else if p.contains(\"data_structures\"){\"data_structures\"} else if p.contains(\"render\"){\"render\"} else if p.contains(\"stdlib\"){\"stdlib\"} else if p.contains(\"ctype\"){\"ctype\"} else {\"misc\"} .to_string() }\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'fn extract_function_prototype(&self, content:&str, func_name:&str)->anyhow::Result<String>{ let re=Regex::new(&format!(r\"(?m)^[^/]*{}[^{{]*{{\", regex::escape(func_name)))?; if let Some(m)=re.find(content){ let proto=m.as_str().trim_end_matches('{').trim(); return Ok(proto.to_string()); } Ok(format!(\"/* Generated prototype for {} */\", func_name)) }\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'fn generate_tags(&self, func_name:&str, content:&str)->Vec<String>{ let mut tags=Vec::new(); if func_name.starts_with(\"ft_str\"){tags.push(\"string\".into());} if func_name.starts_with(\"ft_mem\"){tags.push(\"memory\".into());} if func_name.starts_with(\"ft_is\"){tags.push(\"validation\".into());} if func_name.starts_with(\"ft_to\"){tags.push(\"conversion\".into());} if func_name.contains(\"printf\"){tags.push(\"output\".into());} if func_name.contains(\"scanf\"){tags.push(\"input\".into());} if func_name.contains(\"list\"){tags.push(\"linked_list\".into());} if content.contains(\"malloc\"){tags.push(\"allocation\".into());} if content.contains(\"free\"){tags.push(\"cleanup\".into());} if content.contains(\"while\")||content.contains(\"for\"){tags.push(\"iteration\".into());} if tags.iter().any(|t|t==\"allocation\"){tags.push(\"intermediate\".into());} else {tags.push(\"basic\".into());} tags }\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'fn extract_description(&self, content:&str)->String{ let patterns=[r\"/\\*\\*\\s*(.*?)\\s*\\*/\", r\"/\\*\\s*(.*?)\\s*\\*/\", r\"//\\s*(.*)\" ]; for pat in &patterns{ if let Ok(re)=Regex::new(pat){ if let Some(caps)=re.captures(content){ if let Some(c)=caps.get(1){ let desc=c.as_str().lines().map(|l|l.trim().trim_start_matches('*').trim()).filter(|l|!l.is_empty()&&!l.contains(\"********\")&&!l.contains(\":::      ::::::::\")).collect::<Vec<_>>().join(\" \"); if !desc.is_empty() && desc.len()>10 { return desc; } } } } } \"No description available.\".into() }\n' >> $(DOC_DIR)/src/generator.rs

	@printf '#[derive(Parser)]\n' >> $(DOC_DIR)/src/generator.rs
	@printf '#[command(name=\"doc-generator\")]\n' >> $(DOC_DIR)/src/generator.rs
	@printf '#[command(about=\"Generate documentation for libft\")]\n' >> $(DOC_DIR)/src/generator.rs
	@printf 'struct Args{ #[arg(short,long,default_value=\".\")] source:String, #[arg(short,long,default_value=\"dist\")] output:String }\n' >> $(DOC_DIR)/src/generator.rs

	@printf 'fn main()->anyhow::Result<()> { let args=Args::parse(); println!(\"🔍 Parsing libft source code from: {}\", args.source); let parser=LibftParser::new(args.source); let metadata=parser.parse()?; println!(\"📝 Found {} functions in {} categories\", metadata.functions.len(), metadata.categories.len()); fs::create_dir_all(&args.output)?; let metadata_json=serde_json::to_string_pretty(&metadata)?; fs::write(format!(\"{}/metadata.json\", args.output), metadata_json)?; let html=generate_basic_html(&metadata)?; fs::write(format!(\"{}/index.html\", args.output), html)?; println!(\"✅ Documentation generated in: {}\", args.output); Ok(()) }\n' >> $(DOC_DIR)/src/generator.rs

	@printf 'fn generate_basic_html(m:&LibraryMetadata)->anyhow::Result<String>{ let mut html=String::new(); html.push_str(\"<!DOCTYPE html>\\n<html lang=\\\"en\\\">\\n<head>\\n<meta charset=\\\"UTF-8\\\">\\n<meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0\\\">\\n\"); html.push_str(&format!(\"<title>{} Documentation</title>\\n\", m.name)); html.push_str(\"<style>\\n\"); html.push_str(include_str!(\"../static/styles.css\")); html.push_str(\"</style>\\n</head>\\n<body>\\n<header>\\n\"); html.push_str(&format!(\"<h1>{} Documentation</h1>\\n<p>{}</p>\\n\", m.name, m.description)); html.push_str(\"</header>\\n<main>\\n<section class=\\\"overview\\\"><h2>Library Overview</h2>\\n\"); html.push_str(&format!(\"<p>Total Functions: {}</p>\\n<p>Categories: {}</p>\\n\", m.functions.len(), m.categories.len())); html.push_str(\"</section>\\n<section class=\\\"categories\\\"><h2>Categories</h2><ul>\\n\"); for c in &m.categories { let cnt=m.functions.values().filter(|f|f.category==*c).count(); html.push_str(&format!(\"<li>{} ({} functions)</li>\\n\", c, cnt)); } html.push_str(\"</ul></section>\\n<section class=\\\"functions\\\"><h2>Functions</h2>\\n\"); let mut grouped=std::collections::BTreeMap::<String,Vec<&FunctionMetadata>>::new(); for f in m.functions.values(){ grouped.entry(f.category.clone()).or_default().push(f);} for fs in grouped.values_mut(){ fs.sort_by(|a,b|a.name.cmp(&b.name)); } for (cat, fs) in grouped{ html.push_str(&format!(\"<h3>{}</h3>\\n<div class=\\\"function-grid\\\">\\n\", cat)); for f in fs { html.push_str(\"<div class=\\\"function-card\\\">\\n\"); html.push_str(&format!(\"<h4>{}</h4>\\n<p>{}</p>\\n\", f.name, f.description)); html.push_str(\"<div class=\\\"tags\\\">\\n\"); for t in &f.tags { html.push_str(&format!(\"<span class=\\\"tag\\\">{}</span>\\n\", t)); } html.push_str(\"</div>\\n\"); html.push_str(&format!(\"<code>{}</code>\\n\", f.prototype)); html.push_str(\"</div>\\n\"); } html.push_str(\"</div>\\n\"); } html.push_str(\"</section>\\n</main>\\n</body>\\n</html>\\n\"); Ok(html) }\n' >> $(DOC_DIR)/src/generator.rs

	@# Ensure styles.css exists
	@printf '%s\n' '/* minimal ensure */' >> /dev/null
	@[ -f "$(DOC_DIR)/static/styles.css" ] || { \
		mkdir -p "$(DOC_DIR)/static"; \
		printf 'body{font-family:sans-serif;margin:0;background:#f8f9fa;color:#333}header{padding:2rem;background:#667eea;background:linear-gradient(135deg,#667eea 0%%,#764ba2 100%%);color:#fff;text-align:center}main{max-width:1200px;margin:0 auto;padding:2rem}.function-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:1rem}.function-card{background:#fff;border:1px solid #e1e8ed;border-radius:6px;padding:1rem}\n' > "$(DOC_DIR)/static/styles.css"; \
	}