# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

An oversized **libft** — a 42-school C library that has grown well past the original
assignment into a general-purpose toolkit (data structures, allocators, IPC, sorting,
a `printf` family, termcap, etc.). It is **vendored into a `42sh` shell project** at
`vendor/libft` (this repo is itself a git submodule of the parent — `.git` here is a
gitlink, not a directory). Much of the API and many constants are shaped by that shell
consumer.

## Build

```bash
make            # or `make all` — builds build/lib/libft.a AND build/lib/libft.so
make clean      # remove build/obj only
make fclean     # remove the whole build/ tree + the libs
make re         # fclean + all
make verif_norm # run 42 norminette over the tree (filtered, with a spinner)
```

- Compiler flags: `-Wall -Wextra -Werror -D_POSIX_C_SOURCE=200809L -DMINISHELL_DEBUG_PROMPT=1 -std=c99 -O3 -fPIC`. `-O3` is deliberate — libft is the shell's hot path (memcpy, vectors, strings), so building it optimized is a shell-wide win (see the Makefile comment and recent `perf(...)` commits).
- **Sources are auto-discovered**: `SRCS = $(shell find ./srcs -name "*.c")`. To add a function, just drop a `.c` under `srcs/<module>/` — no source list to edit. All objects/deps land under `build/` (`build/obj`, `build/metadata`, `build/lib`); the source tree stays clean. `.d` dependency files give correct incremental rebuilds.
- If a build is interrupted (Ctrl-C), the fancy progress spinner can leave stray background `sleep` processes. `scripts/safe_make.sh` / `scripts/build.sh` wrap `make` and kill them on exit — use them instead of bare `make` if you hit zombie spinners.

## The allocator switch: fn_* and the ft_malloc submodule

`srcs/memory/ft_malloc/` is a **separate git submodule** (`Univers42/ft_malloc`) — a
standalone 4.4BSD-derived `malloc` replacement (`ft_malloc`/`ft_free`/`ft_realloc`/
`ft_calloc`, thread-safe) with its **own** Makefile/includes/tests/CLAUDE.md. **Never edit
it from here**; read `srcs/memory/ft_malloc/CLAUDE.md` before touching it.

libft routes **every** allocation through a compile-time switch in
[include/xalloc.h](include/xalloc.h) so the allocator is chosen at build time:

- `fn_malloc` / `fn_calloc` / `fn_realloc` / `fn_free` — the call-site API. These are
  `static inline` (the 42 norm forbids function-like macros); a global `#ifdef` picks
  `FN_BACKEND_*` object-like macros = `ft_*` (submodule present) **or** libc
  `malloc/...` (absent). **Use `fn_*` for all new allocation in libft** — never raw
  `malloc`/`free`.
- The Makefile auto-detects the submodule (`FTM_SRCS`): present ⇒ it compiles the
  submodule's `src/**/*.c` into `libft.a`/`.so` (dedicated rule, the submodule's own
  flags + `-pthread`, **not** `-std=c99`) and defines `HAVE_FT_MALLOC`; absent ⇒ pure
  libc. The submodule is pruned from the main `find` glob.
- `xalloc.h` is **force-included** into every libft TU (`-include include/xalloc.h`) and
  also sets the feature-test macros (`_DEFAULT_SOURCE`, …) before any system header.
- The Makefile generates `include/xalloc_config.h` (gitignored) that `#define`s
  `HAVE_FT_MALLOC` to match how libft was built, and `xalloc.h` includes it. So **any
  consumer of `libft.h` inherits the same choice automatically** — no `-D` needed, and you
  can't free an `ft_malloc` block with libc `free`.

Related renames done for this switch (avoid re-colliding with the submodule's `ft_*`):
the old `x*` safe-wrappers are now **`xfn_malloc`/`xfn_calloc`/`xfn_realloc`/`xfn_free`**
(declared in `ft_memory.h`, defined in `srcs/memory/xmalloc.c`; `xrealloc`/`xrealloc3`
were unified into one `xfn_realloc`). libft's own 3-arg realloc helper is now
**`fn_realloc3(ptr, old, new)`** (was `ft_realloc`, which clashed with the submodule's
2-arg one). libft's `ft_calloc` body is guarded by `#ifndef HAVE_FT_MALLOC` so the
submodule supplies `ft_calloc` in ft-mode.

> **Cross-boundary caveat:** memory must be freed by the same allocator that made it. If a
> *consumer* (e.g. the parent 42sh) gets a buffer from libft (`ft_split`, `ft_strdup`, …)
> built in ft-mode, it must free it with `fn_free`/`ft_free`, not libc `free`. The
> generated header makes the parent's `fn_*` calls route correctly, but the parent's raw
> `malloc`/`free` stay libc — migrate those too for full ft_malloc coverage.

> CI note: `.github/workflows/build.yml` verifies a root-level `libft.a`, but the Makefile
> emits `build/lib/libft.a` — the CI verify step is stale relative to the build layout.

## Testing

Comparison test suite under `tests/` validates that libft behaves **identically with libc
malloc and with ft_malloc, leaking nothing**:

```bash
make run_test=all          # build BOTH allocator archives, run every test under each
make run_test=test_string  # run one test (tests/units/test_<name>.c)
make bench                 # libc-vs-ft_malloc benchmark (alloc-heavy workloads)
```

How it works ([tests/run_tests.sh](tests/run_tests.sh)): builds libft twice — a libc
archive (`build/test/libc/lib/libft.a`) and an ft_malloc archive
(`build/test/ft/lib/libft.a`), cached side-by-side via `BUILD_DIR=…` +
`EXTRA_CFLAGS=-DXALLOC_NO_CONFIG` (so the shared generated `xalloc_config.h` doesn't clash;
the mode is taken purely from `-D`). Each `tests/units/test_*.c` is compiled against both
(binaries → `build/bin/test/<name>_{libc,ft}`) and must satisfy **4 gates**: libc exit 0,
ft exit 0, **identical stdout** (functional equivalence), and **valgrind clean** (libc-side
leak/UB oracle). The ft side's leak oracle is `malloc_live_bytes()` (checked in `t_end()`).

Writing a test: `#include "libft_test.h"` ([tests/harness/libft_test.h](tests/harness/libft_test.h)),
use `t_check_*`/`t_rand`, `return (t_end())`. **STDOUT must be logical-only** (no pointers/
timestamps) or the cross-allocator diff breaks; **free everything** with `fn_free` (or the
type's destroy fn) or the ft leak gate fails. Tests link with `-lm` (some sorts use `log2`)
and the ft variant with `-pthread`. The `mk/common.mk` `test`/`valgrind` targets are legacy
(not included by the root Makefile). The ft_malloc submodule also has its own separate
test/bench/leakcheck suite — see its CLAUDE.md.

**Known pre-existing libft bugs** surfaced by the suite (function bugs, identical under both
allocators, so they don't affect the allocator-equivalence result; tests work around them
and document them in-file): `ft_strncasecmp` off-by-one + `n==0` underflow; `ft_strchr(s,'\0')`
returns NULL not the terminator; `ft_strnstr` derefs needle before the `n==0` check;
`vec_push_int` ignores `elem_size`; the `ds/array` module crashes on sequential
append-then-`array_reference` (NULL `lastref`); `ft_lstclear(NULL,…)` NULL-derefs;
`hash_set` stores keys by pointer (no copy → dangling for stack keys); `fn_realloc3(p,old,0)`
returns NULL without freeing `p`.

## Architecture

**Header aggregation.** `libft.h` at the repo root is the single public entry point. It
pulls in the **umbrella headers** in `include/` (`ds.h`, `ft_string.h`, `ft_memory.h`,
`ft_stdio.h`, `ft_sort.h`, `ft_math.h`, `ft_ctype.h`, `ft_wctype.h`, `ft_termcap.h`,
`ft_time.h`, `ft_render.h`, `ft_debug.h`, `ft_stdlib.h`, `ft_stddef.h`, `ft_colors.h`,
`ipc.h`, `system.h`). Each umbrella header is thin — it mostly `#include`s the **real
per-component headers, which live next to their source** under `srcs/<module>/<component>/`
(e.g. `ds.h` → `srcs/ds/vector/ft_vector.h`, `srcs/ds/hash/hash.h`, …). **To expose new
API:** add the prototype to the component header under `srcs/`, and make sure the matching
`include/ft_*.h` umbrella pulls it in.

**`ft_stddef.h` is the shared vocabulary.** Project-wide typedefs (`t_size`, `t_ssize`,
`t_prec`, `t_qty`, …), the bool/order/sign/side/type/align enums, and — notably — the large
`t_state` enum plus constants that are **shell-specific** (lexer states like `ST_IN_SQUOTE`,
exit codes `COMMAND_NOT_FOUND=127` / `EXE_PERM_DENIED=126`, `PROMPT`, `HIST_FILE`). These
exist because this libft backs the 42sh; expect shell concerns to leak into the "stddef".

**Modules (under `srcs/`):**
- `ds/` — array, vector (growable), hash table, singly/doubly/circular linked lists, queue, dyn_string, bitmap.
- `memory/` — two layers: (1) **in-tree allocators** `memalloc/{arena,pool,slab,lifoba}` that *are* compiled into libft and surfaced via `ft_memory.h`; (2) the **ft_malloc submodule** (separate, see above). `ft_memory.h` also defines `t_mem_tracker` + `safe_malloc`/`cleanup_all` and inline generic destructors (`destroy`, `buffer_destroy`).
- `stdio/` — a `printf` family: `format/` (spec parser + `printf_int/str/ptr/hex`, vprinter) and `output/` (buffered writers, `flush`, fd writers, `claptrap`).
- `string/`, `ctype/`, `wctype/` (locale-aware), `std/` (atoi/itoa/calloc/`strto*`), `sort/` (~16 algorithms: quick/merge/heap/radix/tim/counting/…), `math/algebra/`, `time/`, `termcap/`, `render/`, `debug/` (hex/ascii/array dumps).
- `ipc/` — pipes, fifo, message_queue, sockets, buses, semaphores, signals.

## Conventions

- **42 norminette is enforced** (`make verif_norm`). Every file carries the 42 header banner; types use `t_`/`s_`/`e_` prefixes; functions stay short (≤25 lines, ≤5 args). New code must keep this.
- `-Werror`: warnings are build failures.
- **Trust the code over `README.md`.** The root `README.md` mostly documents the *parent 42sh* project's directory tree (render/camera, lex, parser, framework_test, math/geometry…) — most of which does **not** exist in this libft repo. Verify any structural claim against `srcs/` before relying on it.
