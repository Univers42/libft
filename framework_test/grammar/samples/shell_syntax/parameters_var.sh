#!/bin/sh
# positional parameters and shifting
echo "All args as single word: \"$*\""
printf 'Each arg: [%s]\n' "$@"

# simulate receiving args
set -- one "two three" four
echo "after set -- : \$#=$#"
shift
echo "after shift: \$1='$1', remaining \$#=$#"

# iterate with "$@"
for p in "$@"; do
  echo "param: $p"
done
