#!/bin/sh
# duplicate original stdin to fd 3 and demonstrate reading from both stdin and fd3

TMP=/tmp/_dup_in_test
printf 'orig-line1\norig-line2\n' > "$TMP"

# save original stdin on fd 3 (open file for reading)
exec 3<"$TMP"

# simulate different stdin (here-doc) and read from it
read -r from_stdin <<EOF
stdin-line
EOF
printf 'from stdin: [%s]\n' "$from_stdin"

# read from saved original stdin via fd3
read -r from_fd3 <&3
printf 'from fd3 (saved original): [%s]\n' "$from_fd3"

# close fd3
exec 3<&-
