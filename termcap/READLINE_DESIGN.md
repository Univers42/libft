# Readline Design (minimal, termcap-backed)

## Modules

- rl/keys: decode raw bytes -> key codes
- rl/history: manage history list and cursor
- rl/buffer: editing primitives (insert, delete, cursor move)
- rl/display: efficient redisplay / handle wrapping
- rl/core (readline): orchestrator, raw mode, integrates above modules
- termcap: your existing library (used by display and core)

## Data flow

User input -> rl/keys -> rl/core -> rl/buffer -> rl/display -> tty

## Development plan (iterative)

1. keys + history (done in this commit)
2. buffer + display (next)
3. integrate into core readline (refactor existing readline.c)
4. resize handling and signal integration
5. optional features: kill/yank/undo, incremental search, completion

## Minimal key map (provided by rl/keys)

- Printable chars: ASCII code >= 32
- RL_KEY_LEFT, RL_KEY_RIGHT, RL_KEY_UP, RL_KEY_DOWN
- RL_KEY_HOME, RL_KEY_END, RL_KEY_DELETE, RL_KEY_BACKSPACE
- Ctrl combos: e.g., CTRL('A') for start, CTRL('E') for end, CTRL('L') to clear
- Returns literal ASCII values for simple keys

## Example: using modules

- rl_history_add("ls -la");
- k = rl_read_key(fd);
- if (k == RL_KEY_LEFT) buffer_move_left();
- if (k is printable) buffer_insert(ch) and display_insert(ch)

## Next steps

Implement rl/buffer and rl/display, then refactor readline.c to use them.
