#!/bin/bash
greet() {
  local name="$1"
  echo "Hello, $name"
}
greet "Alice"
( echo "in subshell: $$"; ls /nonexistent >/dev/null 2>&1 || echo "ls failed" )
# capture output of function
out=$(greet Bob)
echo "Captured: $out"
