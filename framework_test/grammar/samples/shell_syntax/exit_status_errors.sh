#!/bin/sh
# Demonstrate error trapping and "set -e" behavior

# ERR trap is shell-dependent; POSIX sh may not support trap on ERR similarly to bash
trap 'echo "ERR trap: status=$?"; exit 2' ERR 2>/dev/null || true

# exit on error
set -e

echo "about to run a failing command"
false    # this will trigger set -e and the ERR trap (if supported), exiting the script

# ...this line is not reached when set -e is active
echo "after-false (not reached with set -e)"
