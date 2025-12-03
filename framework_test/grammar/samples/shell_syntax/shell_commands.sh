#!/bin/sh
# builtins vs functions vs external commands

# function named 'echo' shadows builtin in some shells
echo() { command printf '[func-echo] %s\n' "$*"; }
echo "hello"         # calls function
command echo "hi"    # 'command' runs the original builtin/external echo

# run an external command explicitly (if present)
cmd_path=$(command -v ls 2>/dev/null || true)
printf 'ls path: [%s]\n' "$cmd_path"

# exec replaces the shell with the given command (tokenization example)
# exec true  # uncomment to test replacing the shell
