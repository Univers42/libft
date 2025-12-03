#!/bin/sh
# tricky conditionals and quoting
if [ "x$VAR" = "x" ]; then
  echo empty
fi

# weird eval and printf usage
cmd='printf "%s\n" "a b"'
eval "$cmd"
