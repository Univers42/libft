# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/10 19:03:45 by marvin            #+#    #+#              #
#    Updated: 2026/08/05 17:28:16 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/10 19:03:45 by marvin            #+#    #+#              #
#    Updated: 2026/08/05 17:45:00 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# voluntarily Makefile is simpler to debug quicker the problems on this libft..

# `make run_test=all` or `make run_test=test_string` runs the comparison suite
# (libc vs ft_malloc: same results + no leaks). Defining run_test on the command
# line redirects the default goal to the test orchestrator.
ifdef run_test
.DEFAULT_GOAL := _run_tests
endif

CC = cc

# ---------------------------------------------------------------------------- #
# Toolchain detection (SINGLE source of truth).                                #
#                                                                               #
# Everything downstream that cares whether we're on gcc or clang — the LTO      #
# flags, the archive tools — reads CC_IS_GCC / CC_IS_CLANG / AR / RANLIB from   #
# here. Nothing else in this file is allowed to redefine AR/RANLIB/ARFLAGS;     #
# that duplication is exactly what caused the "archive has no index" bug       #
# (a later plain `AR = ar rcs` was silently clobbering this detection).         #
# ---------------------------------------------------------------------------- #
CC_IS_GCC   := $(shell $(CC) --version 2>/dev/null | grep -qi gcc   && echo 1)
CC_IS_CLANG := $(shell $(CC) --version 2>/dev/null | grep -qi clang && echo 1)

# Archive tools must match what the compiler puts in .o files under LTO. GNU
# ar/ranlib cannot index LLVM bitcode-only objects — that's the exact failure
# ("archive has no index; run ranlib to add one") this block exists to avoid.
# clang -> prefer llvm-ar/llvm-ranlib (can read pure bitcode directly).
# gcc   -> plain ar/ranlib is fine, because -ffat-lto-objects below keeps
#          GCC's LTO objects carrying normal ELF symbol tables too.
ifeq ($(CC_IS_CLANG),1)
  AR     := $(shell command -v llvm-ar 2>/dev/null || echo ar)
  RANLIB := $(shell command -v llvm-ranlib 2>/dev/null || echo ranlib)
else
  AR     := ar
  RANLIB := ranlib
endif
ARFLAGS := rcs

# -O3: libft is the hot path for the whole shell (memcpy, vectors, strings).
# Building it unoptimized (-O0) made every consumer pay; optimizing it is a
# major, shell-wide win.
CFLAGS = -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -DMINISHELL_DEBUG_PROMPT=1 -std=c99 -O3

# ---------------------------------------------------------------------------- #
# LTO, decided by PROBE rather than by compiler name.                          #
#                                                                               #
# Why LTO at all (OPT / SAFE=0 tree only): shipping bytecode next to the code   #
# lets the shell's own -flto link inline vec_push/ft_strlen/fixed-size          #
# ft_memcpy into its hot loops — profiling showed every ft_* call was a real    #
# call because the archive carried no LTO sections. The SAFE=1 tree stays       #
# LTO-free on purpose: GCC's linker plugin processes LTO sections even on a     #
# plain link, and that perturbed the ASan/LeakSanitizer build's accounting.     #
#                                                                               #
# Why a probe: guessing from $(CC) is wrong on real machines and fails FAR      #
# from its cause. clang below 17 ACCEPTS -ffat-lto-objects while only warning   #
# "optimization flag '-ffat-lto-objects' is not supported", then emits pure     #
# LLVM bitcode anyway. GNU ar happily writes an unindexable archive, and the    #
# breakage only surfaces later in the CONSUMER's link as "archive has no        #
# index; run ranlib to add one". So we do not guess: we build a throwaway       #
# archive with the candidate flags and try to link a program against it.        #
#                                                                               #
# If that end-to-end probe fails, LTO is dropped entirely. Losing cross-archive #
# inlining costs some speed; shipping an archive nobody can link costs the      #
# whole build. Correctness first, and the warning below says how to get the     #
# speed back (install llvm-ar/llvm-ranlib).                                     #
#                                                                               #
# LTO_FLAGS is shared with FTM_CFLAGS below so the ft_malloc submodule's        #
# objects follow the exact same decision and never end up as the odd one out    #
# in the final archive.                                                         #
# ---------------------------------------------------------------------------- #

# Probing spawns compilers, so skip it when the goal only deletes files. Without
# this a plain `make fclean` pays for the probe AND prints its warning while
# doing nothing but rm -rf. `make fclean all` still probes: filter-out leaves
# `all` behind. An empty goal list means the default target, so treat it as `all`.
PROBE_GOALS := $(filter-out clean fclean,$(or $(MAKECMDGOALS),all))

LTO_FLAGS :=
ifneq ($(SAFE),1)
ifneq ($(PROBE_GOALS),)

LTO_TRY := -flto
# Fat objects keep a normal ELF symbol table alongside the bytecode, which is
# what lets a plain GNU ar index them. Only worth asking for when we are NOT
# archiving with llvm-ar (which reads pure bitcode natively). GCC has supported
# it for years; clang only since 17 — hence the probe, not an ifeq on CC.
ifeq ($(AR),ar)
LTO_TRY += -ffat-lto-objects
endif

# compile -> archive -> ranlib -> link, with -Werror so a flag that merely
# warns today cannot silently poison the real -Werror build later.
LTO_OK := $(shell d=$$(mktemp -d 2>/dev/null) || exit 0; \
	printf 'int ft_lto_probe(void){return 0;}\n' > $$d/a.c; \
	printf 'int ft_lto_probe(void);\nint main(void){return ft_lto_probe();}\n' \
		> $$d/m.c; \
	$(CC) -Werror $(LTO_TRY) -c $$d/a.c -o $$d/a.o >/dev/null 2>&1 \
	&& $(AR) $(ARFLAGS) $$d/libprobe.a $$d/a.o >/dev/null 2>&1 \
	&& $(RANLIB) $$d/libprobe.a >/dev/null 2>&1 \
	&& $(CC) -Werror $(LTO_TRY) $$d/m.c $$d/libprobe.a -o $$d/probe \
		>/dev/null 2>&1 \
	&& echo 1; \
	rm -rf $$d)

ifeq ($(LTO_OK),1)
LTO_FLAGS := $(LTO_TRY)
else
$(warning libft: this toolchain cannot produce a linkable LTO archive \
[$(CC) + $(AR)] — building WITHOUT LTO. The library is correct, just a little \
slower. To restore it, install llvm-ar/llvm-ranlib e.g. `apt install llvm`.)
endif

endif
endif

CFLAGS += $(LTO_FLAGS)

PICFLAG = -fPIC
CFLAGS += $(PICFLAG)

# gold links LLVM bitcode via its plugin, so it was pinned here for the LTO
# shared-library link. It is NOT a given, though: plenty of machines never
# install it, and binutils 2.44 removed it outright. Probing keeps the build
# working on a bare toolchain instead of dying on an unknown -fuse-ld target.
ifneq ($(PROBE_GOALS),)
GOLD_OK := $(shell d=$$(mktemp -d 2>/dev/null) || exit 0; \
	printf 'int main(void){return 0;}\n' > $$d/g.c; \
	$(CC) -fuse-ld=gold $$d/g.c -o $$d/g >/dev/null 2>&1 && echo 1; \
	rm -rf $$d)
ifeq ($(GOLD_OK),1)
LDFLAGS += -fuse-ld=gold
endif
endif
BUILD_DIR ?= build
BIN_DIR = $(BUILD_DIR)/bin
LIB_DIR = $(BUILD_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj
META_DIR = $(BUILD_DIR)/metadata

NAME = $(LIB_DIR)/libft.a
STATIC_LIB = $(LIB_DIR)/libft.a
SHARED_LIB = $(LIB_DIR)/libft.so

# ---------------------------------------------------------------------------- #
# ft_malloc integration (optional git submodule at srcs/memory/ft_malloc)       #
#                                                                               #
# Checked out (git clone --recursive / git submodule update --init): its        #
# src/*.c are compiled into libft and the fn_* macros (include/xalloc.h) route   #
# every allocation to ft_malloc. Absent: fn_* falls back to libc malloc/free.    #
# The submodule is compiled with ITS OWN flags (not -std=c99 clean, needs        #
# -pthread) by a dedicated rule, and is kept out of the main source glob.        #
# ---------------------------------------------------------------------------- #
FTM_DIR  = srcs/memory/ft_malloc
FTM_SRCS = $(shell find $(FTM_DIR)/src -name "*.c" 2>/dev/null)
CONFIG_H = include/xalloc_config.h
# SAFE=1 forces the libc allocator even when ft_malloc sources are present
# (the parent shell passes SAFE down). SAFE=0 / unset keeps the custom heap.
ifneq ($(strip $(FTM_SRCS)),)
ifneq ($(SAFE),1)
HAVE_FTM := 1
CFLAGS += -DHAVE_FT_MALLOC -pthread

# Shares LTO_FLAGS with the main CFLAGS (see comment above it) instead of
# deciding -flto/-ffat-lto-objects a second time — that duplication used to
# hardcode -flto unconditionally and only add -ffat-lto-objects for gcc,
# which meant FTM objects stayed pure bitcode on clang even when the main
# archive had been fixed to expect fat objects. One decision, reused twice.
FTM_CFLAGS = -Wall -Wextra -Werror -fPIC -O3 -pthread -I$(FTM_DIR)/include $(LTO_FLAGS)

FTM_OBJS = $(FTM_SRCS:%.c=$(OBJ_DIR)/%.o)
endif
endif

# Force the allocator switch into every libft TU, regardless of its own
# includes. ft_malloc's own sources use the separate rule below and never see it.
FORCE_XALLOC = -include include/xalloc.h

# Escape hatch for callers (e.g. the test harness) to inject extra flags such as
# -DXALLOC_NO_CONFIG when building cached per-allocator archives.
CFLAGS += $(EXTRA_CFLAGS)

# main libft sources = everything under srcs/ EXCEPT the ft_malloc submodule
SRCS = $(shell find ./srcs -path "./$(FTM_DIR)" -prune -o -name "*.c" -print)
INCS = -I./include -I.
RM = rm -rf
TOTAL = $(words $(SRCS) $(FTM_SRCS))
DEPFLAGS = -MMD -MP -MF
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
DEPS = $(SRCS:%.c=$(META_DIR)/%.d)
ALL_OBJS = $(OBJS) $(FTM_OBJS)

all : $(STATIC_LIB) $(SHARED_LIB)
	@printf "\r\033[K" >&2
	@printf "\n" >&2
	@printf "  \033[1;33m●\033[0m \033[1;33mlibft\033[0m \033[33m─\033[0m \033[32mcompilation successful\033[0m\n" >&2
	@printf "  \033[33m────────────────────────────────────────\033[0m\n" >&2
	@printf "  \033[32m✓\033[0m \033[37m%s\033[0m\n" "$(STATIC_LIB)" >&2
	@printf "  \033[32m✓\033[0m \033[37m%s\033[0m\n" "$(SHARED_LIB)" >&2
	@printf "\n" >&2

# Bake the active allocator choice into a header so every consumer of libft.h
# inherits it (no -D needed). Rewritten only when the mode actually changes, so
# stable builds don't trigger a recompile.
$(CONFIG_H): FORCE
	@mkdir -p $(dir $@)
	@{ \
		echo '/* AUTO-GENERATED by the libft Makefile — do not edit. */'; \
		echo '#ifndef XALLOC_CONFIG_H'; \
		echo '# define XALLOC_CONFIG_H'; \
		if [ -n "$(HAVE_FTM)" ]; then \
			echo '# ifndef HAVE_FT_MALLOC'; \
			echo '#  define HAVE_FT_MALLOC 1'; \
			echo '# endif'; \
		else \
			echo '/* ft_malloc submodule absent: fn_* -> libc malloc/free */'; \
		fi; \
		echo '#endif'; \
	} > $@.tmp
	@if ! cmp -s $@.tmp $@; then mv $@.tmp $@; else $(RM) $@.tmp; fi

FORCE:
.PHONY: FORCE

# Ensure the config header exists before any libft TU compiles (first build);
# content-change rebuilds are handled by the generated .d dependencies.
$(OBJS): | $(CONFIG_H)

$(STATIC_LIB): $(ALL_OBJS)
	@mkdir -p $(LIB_DIR)
	@$(AR) $(ARFLAGS) $@ $^
	@$(RANLIB) $@

# Uses the probed $(LTO_FLAGS)/$(LDFLAGS) rather than a hardcoded
# `-flto -fuse-ld=gold`: those two were asserted unconditionally here, so this
# rule asked for LTO even in the deliberately LTO-free SAFE=1 tree and demanded
# a linker the host may not have.
$(SHARED_LIB): $(ALL_OBJS)
	@mkdir -p $(LIB_DIR)
	@$(CC) $(LTO_FLAGS) $(LDFLAGS) -shared -pthread -o $@ $^

# ft_malloc submodule: compiled with its own flags + include path, no force
# header, no -std=c99 (more-specific pattern wins over the generic rule below).
$(OBJ_DIR)/$(FTM_DIR)/%.o : $(FTM_DIR)/%.c
	@mkdir -p $(dir $@)
	@if ! $(CC) $(FTM_CFLAGS) -c $< -o $@; then \
		printf "\n\033[1;31m  ⬢ FT_MALLOC ERROR ⬢\033[0m\n  \033[90m▸\033[0m \033[37m%s\033[0m\n\n" "$<" >&2; \
		exit 1; \
	fi

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@meta_file=$(META_DIR)/$<; meta_file=$${meta_file%.c}.d; mkdir -p "$$(dirname "$$meta_file")"; \
	if ! $(CC) $(CFLAGS) $(FORCE_XALLOC) $(INCS) $(DEPFLAGS) "$$meta_file" -c $< -o $@; then \
	    printf "\n\033[1;31m  ⬢ SYSTEM ERROR ⬢\033[0m\n  \033[90m▸\033[0m \033[37m%s\033[0m\n\n" "$<" >&2; \
	    exit 1; \
	fi
	@mkdir -p $(META_DIR)
	@find . -maxdepth 1 -type f -name ".*" ! -name ".gitignore" ! -name ".gitattributes" ! -name ".gitmodules" ! -name ".git" ! -name "." ! -name ".." -print0 | xargs -0 -r -I{} mv {} $(META_DIR) 2>/dev/null || true
	@count=$$(find $(OBJ_DIR) -name "*.o" 2>/dev/null | wc -l); \
	percent=$$(($$count * 100 / $(TOTAL))); \
	filled=$$(($$percent * 30 / 100)); \
	bar=""; \
	for i in $$(seq 1 30); do \
	    if [ $$i -le $$filled ]; then \
	        bar="$$bar\033[1;36m▰\033[0m"; \
	    else \
	        bar="$$bar\033[90m▱\033[0m"; \
	    fi; \
	done; \
	spin=$$(($$count % 8)); \
	case $$spin in \
	    0) spinner="\033[1;35m⠋\033[0m";; \
	    1) spinner="\033[1;36m⠙\033[0m";; \
	    2) spinner="\033[1;33m⠹\033[0m";; \
	    3) spinner="\033[1;31;5m⠸\033[0m";; \
	    4) spinner="\033[1;32m⠼\033[0m";; \
	    5) spinner="\033[1;34m⠴\033[0m";; \
	    6) spinner="\033[1;37m⠦\033[0m";; \
	    7) spinner="\033[1;96m⠧\033[0m";; \
	esac; \
	printf "\r  $$spinner \033[1;37mLIBFT\033[0m  %b  \033[1;36m%3d%%\033[0m \033[90m%d/%d\033[0m " \
	    "$$bar" $$percent $$count $(TOTAL) >&2;

clean:
	@$(RM) $(OBJ_DIR)
	
fclean : clean
	@$(RM) $(STATIC_LIB) $(SHARED_LIB) $(BUILD_DIR) $(CONFIG_H)

re : fclean all

# Comparison test suite (driven by `make run_test=all` / `make run_test=<name>`).
# bench: `make bench` runs the libc-vs-ft_malloc benchmark.
_run_tests:
	@bash tests/run_tests.sh "$(run_test)"

bench:
	@bash tests/bench.sh

verif_norm:
	@printf "\n  \033[1;36m▸\033[0m Running norminette" >&2; \
	output="$$( \
	    norminette 2>&1 | grep -v 'OK!' | grep -v 'US' & \
	    pid=$$!; \
	    while kill -0 $$pid 2>/dev/null; do \
	        for dots in '.' '..' '...' '....' '.....' '......'; do \
	            printf "\r  \033[1;36m▸\033[0m Running norminette\033[1;35m%-6s\033[0m" "$$dots" >&2; \
	            sleep 0.1; \
	            kill -0 $$pid 2>/dev/null || break; \
	        done; \
	    done; \
	    wait $$pid)"; \
	if [ -z "$$output" ]; then \
	    printf "\r\033[K  \033[1;32m✓\033[0m \033[1;37mNORM CHECK PASSED\033[0m\n\n"; \
	else \
	    printf "\r\033[K  \033[1;31m✗\033[0m \033[1;37mNORM VIOLATIONS:\033[0m\n\n\033[37m%s\033[0m\n\n" "$$output"; \
	fi

.PHONY: all fclean clean re _run_tests bench verif_norm

-include $(DEPS)


# # voluntarily Makefile is simpler to debug quicker the problems on this libft..

# # `make run_test=all` or `make run_test=test_string` runs the comparison suite
# # (libc vs ft_malloc: same results + no leaks). Defining run_test on the command
# # line redirects the default goal to the test orchestrator.
# ifdef run_test
# .DEFAULT_GOAL := _run_tests
# endif

# CC = cc
# # -O3: libft is the hot path for the whole shell (memcpy, vectors, strings).
# # Building it unoptimized (-O0) made every consumer pay; optimizing it is a
# # major, shell-wide win.
# CFLAGS = -Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -DMINISHELL_DEBUG_PROMPT=1 -std=c99 -O3
# # -flto -ffat-lto-objects, OPT (SAFE=0) tree ONLY: ship LTO bytecode next to
# # regular code so the shell's -flto link inlines vec_push/ft_strlen/fixed-
# # size ft_memcpy into its hot loops (profiling showed every ft_* call was a
# # real call — the archive had no LTO sections). The SAFE=1 tree must stay
# # LTO-free: GCC's linker plugin processes LTO sections even on a plain link,
# # and that perturbed the ASan/LeakSanitizer debug build's accounting.

# CC_IS_GCC := $(shell $(CC) --version 2>/dev/null | grep -qi gcc && echo 1)

# ifneq ($(SAFE),1)
# 	CFLAGS += -flto
# ifeq ($(CC_IS_GCC),1)
# 	CFLAGS += -ffat-lto-objects
# endif
# endif

# PICFLAG = -fPIC
# CFLAGS += $(PICFLAG)
# LDFLAGS+= -fuse-ld=gold
# BUILD_DIR ?= build
# BIN_DIR = $(BUILD_DIR)/bin
# LIB_DIR = $(BUILD_DIR)/lib
# OBJ_DIR = $(BUILD_DIR)/obj
# META_DIR = $(BUILD_DIR)/metadata

# NAME = $(LIB_DIR)/libft.a
# STATIC_LIB = $(LIB_DIR)/libft.a
# SHARED_LIB = $(LIB_DIR)/libft.so

# # ---------------------------------------------------------------------------- #
# # ft_malloc integration (optional git submodule at srcs/memory/ft_malloc)       #
# #                                                                               #
# # Checked out (git clone --recursive / git submodule update --init): its        #
# # src/*.c are compiled into libft and the fn_* macros (include/xalloc.h) route   #
# # every allocation to ft_malloc. Absent: fn_* falls back to libc malloc/free.    #
# # The submodule is compiled with ITS OWN flags (not -std=c99 clean, needs        #
# # -pthread) by a dedicated rule, and is kept out of the main source glob.        #
# # ---------------------------------------------------------------------------- #
# FTM_DIR  = srcs/memory/ft_malloc
# FTM_SRCS = $(shell find $(FTM_DIR)/src -name "*.c" 2>/dev/null)
# CONFIG_H = include/xalloc_config.h
# # SAFE=1 forces the libc allocator even when ft_malloc sources are present
# # (the parent shell passes SAFE down). SAFE=0 / unset keeps the custom heap.
# ifneq ($(strip $(FTM_SRCS)),)
# ifneq ($(SAFE),1)
# HAVE_FTM := 1
# CFLAGS += -DHAVE_FT_MALLOC -pthread

# FTM_CFLAGS = -Wall -Wextra -Werror -fPIC -O3 -flto -pthread -I$(FTM_DIR)/include
# ifeq ($(CC_IS_GCC),1)
# FTM_CFLAGS += -ffat-lto-objects
# endif

# FTM_OBJS = $(FTM_SRCS:%.c=$(OBJ_DIR)/%.o)
# endif
# endif


# # Force the allocator switch into every libft TU, regardless of its own
# # includes. ft_malloc's own sources use the separate rule below and never see it.
# FORCE_XALLOC = -include include/xalloc.h

# # Escape hatch for callers (e.g. the test harness) to inject extra flags such as
# # -DXALLOC_NO_CONFIG when building cached per-allocator archives.
# CFLAGS += $(EXTRA_CFLAGS)

# # main libft sources = everything under srcs/ EXCEPT the ft_malloc submodule
# SRCS = $(shell find ./srcs -path "./$(FTM_DIR)" -prune -o -name "*.c" -print)
# INCS = -I./include -I.
# AR = ar rcs
# RM = rm -rf
# TOTAL = $(words $(SRCS) $(FTM_SRCS))
# DEPFLAGS = -MMD -MP -MF
# OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
# DEPS = $(SRCS:%.c=$(META_DIR)/%.d)
# ALL_OBJS = $(OBJS) $(FTM_OBJS)


# CC_IS_CLANG := $(shell $(CC) --version 2>/dev/null | grep -qi clang && echo 1)

# ifeq ($(CC_IS_CLANG),1)
#   AR      := $(shell command -v llvm-ar 2>/dev/null || echo ar)
#   RANLIB  := $(shell command -v llvm-ranlib 2>/dev/null || echo ranlib)
# else
#   AR      := ar
#   RANLIB  := ranlib
# endif
# ARFLAGS := rcs


# all : $(STATIC_LIB) $(SHARED_LIB)
# 	@printf "\r\033[K" >&2
# 	@printf "\n" >&2
# 	@printf "  \033[1;33m●\033[0m \033[1;33mlibft\033[0m \033[33m─\033[0m \033[32mcompilation successful\033[0m\n" >&2
# 	@printf "  \033[33m────────────────────────────────────────\033[0m\n" >&2
# 	@printf "  \033[32m✓\033[0m \033[37m%s\033[0m\n" "$(STATIC_LIB)" >&2
# 	@printf "  \033[32m✓\033[0m \033[37m%s\033[0m\n" "$(SHARED_LIB)" >&2
# 	@printf "\n" >&2

# # Bake the active allocator choice into a header so every consumer of libft.h
# # inherits it (no -D needed). Rewritten only when the mode actually changes, so
# # stable builds don't trigger a recompile.
# $(CONFIG_H): FORCE
# 	@mkdir -p $(dir $@)
# 	@{ \
# 		echo '/* AUTO-GENERATED by the libft Makefile — do not edit. */'; \
# 		echo '#ifndef XALLOC_CONFIG_H'; \
# 		echo '# define XALLOC_CONFIG_H'; \
# 		if [ -n "$(HAVE_FTM)" ]; then \
# 			echo '# ifndef HAVE_FT_MALLOC'; \
# 			echo '#  define HAVE_FT_MALLOC 1'; \
# 			echo '# endif'; \
# 		else \
# 			echo '/* ft_malloc submodule absent: fn_* -> libc malloc/free */'; \
# 		fi; \
# 		echo '#endif'; \
# 	} > $@.tmp
# 	@if ! cmp -s $@.tmp $@; then mv $@.tmp $@; else $(RM) $@.tmp; fi

# FORCE:
# .PHONY: FORCE

# # Ensure the config header exists before any libft TU compiles (first build);
# # content-change rebuilds are handled by the generated .d dependencies.
# $(OBJS): | $(CONFIG_H)

# $(STATIC_LIB): $(ALL_OBJS)
# 	@mkdir -p $(LIB_DIR)
# 	@$(AR) $(ARFLAGS) $@ $^
# 	@$(RANLIB) $@

# $(SHARED_LIB): $(ALL_OBJS)
# 	@mkdir -p $(LIB_DIR)
# 	@$(CC) -flto -fuse-ld=gold -shared -pthread -o $@ $^

# # ft_malloc submodule: compiled with its own flags + include path, no force
# # header, no -std=c99 (more-specific pattern wins over the generic rule below).
# $(OBJ_DIR)/$(FTM_DIR)/%.o : $(FTM_DIR)/%.c
# 	@mkdir -p $(dir $@)
# 	@if ! $(CC) $(FTM_CFLAGS) -c $< -o $@; then \
# 		printf "\n\033[1;31m  ⬢ FT_MALLOC ERROR ⬢\033[0m\n  \033[90m▸\033[0m \033[37m%s\033[0m\n\n" "$<" >&2; \
# 		exit 1; \
# 	fi

# $(OBJ_DIR)/%.o : %.c
# 	@mkdir -p $(dir $@)
# 	@meta_file=$(META_DIR)/$<; meta_file=$${meta_file%.c}.d; mkdir -p "$$(dirname "$$meta_file")"; \
# 	if ! $(CC) $(CFLAGS) $(FORCE_XALLOC) $(INCS) $(DEPFLAGS) "$$meta_file" -c $< -o $@; then \
# 	    printf "\n\033[1;31m  ⬢ SYSTEM ERROR ⬢\033[0m\n  \033[90m▸\033[0m \033[37m%s\033[0m\n\n" "$<" >&2; \
# 	    exit 1; \
# 	fi
# 	@mkdir -p $(META_DIR)
# 	@find . -maxdepth 1 -type f -name ".*" ! -name ".gitignore" ! -name ".gitattributes" ! -name ".gitmodules" ! -name ".git" ! -name "." ! -name ".." -print0 | xargs -0 -r -I{} mv {} $(META_DIR) 2>/dev/null || true
# 	@count=$$(find $(OBJ_DIR) -name "*.o" 2>/dev/null | wc -l); \
# 	percent=$$(($$count * 100 / $(TOTAL))); \
# 	filled=$$(($$percent * 30 / 100)); \
# 	bar=""; \
# 	for i in $$(seq 1 30); do \
# 	    if [ $$i -le $$filled ]; then \
# 	        bar="$$bar\033[1;36m▰\033[0m"; \
# 	    else \
# 	        bar="$$bar\033[90m▱\033[0m"; \
# 	    fi; \
# 	done; \
# 	spin=$$(($$count % 8)); \
# 	case $$spin in \
# 	    0) spinner="\033[1;35m⠋\033[0m";; \
# 	    1) spinner="\033[1;36m⠙\033[0m";; \
# 	    2) spinner="\033[1;33m⠹\033[0m";; \
# 	    3) spinner="\033[1;31;5m⠸\033[0m";; \
# 	    4) spinner="\033[1;32m⠼\033[0m";; \
# 	    5) spinner="\033[1;34m⠴\033[0m";; \
# 	    6) spinner="\033[1;37m⠦\033[0m";; \
# 	    7) spinner="\033[1;96m⠧\033[0m";; \
# 	esac; \
# 	printf "\r  $$spinner \033[1;37mLIBFT\033[0m  %b  \033[1;36m%3d%%\033[0m \033[90m%d/%d\033[0m " \
# 	    "$$bar" $$percent $$count $(TOTAL) >&2;

# clean:
# 	@$(RM) $(OBJ_DIR)
	
# fclean : clean
# 	@$(RM) $(STATIC_LIB) $(SHARED_LIB) $(BUILD_DIR) $(CONFIG_H)

# re : fclean all

# # Comparison test suite (driven by `make run_test=all` / `make run_test=<name>`).
# # bench: `make bench` runs the libc-vs-ft_malloc benchmark.
# _run_tests:
# 	@bash tests/run_tests.sh "$(run_test)"

# bench:
# 	@bash tests/bench.sh

# verif_norm:
# 	@printf "\n  \033[1;36m▸\033[0m Running norminette" >&2; \
# 	output="$$( \
# 	    norminette 2>&1 | grep -v 'OK!' | grep -v 'US' & \
# 	    pid=$$!; \
# 	    while kill -0 $$pid 2>/dev/null; do \
# 	        for dots in '.' '..' '...' '....' '.....' '......'; do \
# 	            printf "\r  \033[1;36m▸\033[0m Running norminette\033[1;35m%-6s\033[0m" "$$dots" >&2; \
# 	            sleep 0.1; \
# 	            kill -0 $$pid 2>/dev/null || break; \
# 	        done; \
# 	    done; \
# 	    wait $$pid)"; \
# 	if [ -z "$$output" ]; then \
# 	    printf "\r\033[K  \033[1;32m✓\033[0m \033[1;37mNORM CHECK PASSED\033[0m\n\n"; \
# 	else \
# 	    printf "\r\033[K  \033[1;31m✗\033[0m \033[1;37mNORM VIOLATIONS:\033[0m\n\n\033[37m%s\033[0m\n\n" "$$output"; \
# 	fi

# .PHONY: all fclean clean re _run_tests bench verif_norm

# -include $(DEPS)
