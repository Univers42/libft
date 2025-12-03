#!/bin/bash
# Demonstrate command search and execution behaviors: PATH, type, command, hash

echo "Initial PATH: $PATH"

# create a temporary executable in /tmp and prepend /tmp to PATH
cat > /tmp/cmd_demo.sh <<'SH'
#!/bin/sh
echo "executed /tmp/cmd_demo.sh"
SH
chmod +x /tmp/cmd_demo.sh
PATH="/tmp:$PATH"
export PATH
echo "Prepended /tmp to PATH"

# show how the shell locates commands
echo "command -v cmd_demo.sh: $(command -v cmd_demo.sh 2>/dev/null || true)"
type -a cmd_demo.sh 2>/dev/null || echo "type: not found"
which cmd_demo.sh 2>/dev/null || echo "which: not available or not found"

# run the found command
cmd_demo.sh

# demonstrate function shadowing a command
cmd_demo.sh() { echo "function: shadowed cmd_demo"; }
echo "After defining function:"
type -a cmd_demo.sh
cmd_demo.sh        # calls function
command cmd_demo.sh  # bypass function and run external/cached command

# demonstrate hashing: cache the path for cmd_demo.sh, then remove the file and still execute
hash -p /tmp/cmd_demo.sh cmd_demo.sh 2>/dev/null || true
echo "Hashed path for cmd_demo.sh (if supported):"
hash | grep cmd_demo.sh 2>/dev/null || echo "no hash entry visible"

# remove underlying file
rm -f /tmp/cmd_demo.sh
echo "/tmp/cmd_demo.sh removed from filesystem"

# if shell uses hashed path, this may still run; otherwise, it should fail
echo "Attempting to run cmd_demo.sh after removal (may succeed if hashed):"
if cmd_demo.sh >/dev/null 2>&1; then
  echo "ran via cached/hashed path"
else
  echo "failed to run (not found or cache invalid)"
fi

# clear the hash table and try again
hash -r 2>/dev/null || true
echo "Cleared hash table (hash -r)"

echo "Attempting to run cmd_demo.sh after hash -r (should fail):"
if cmd_demo.sh >/dev/null 2>&1; then
  echo "unexpectedly ran"
else
  echo "as expected: command not found"
fi

# cleanup: remove PATH entry we added (best-effort)
PATH=${PATH#"/tmp:"}
export PATH
echo "Done."
