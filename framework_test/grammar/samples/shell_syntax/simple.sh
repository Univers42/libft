#!/bin/sh
# simple script: args, loops, conditional
echo "Args: $*"
for a in "$@"; do
  echo "Got: $a"
done
if [ -n "$1" ]; then
  echo "First arg exists"
else
  echo "No args"
fi
