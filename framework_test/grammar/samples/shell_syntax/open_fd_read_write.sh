#!/bin/sh
# open a file on fd 5 for read and write, demonstrate reading and writing via the descriptor

TARGET=/tmp/_fd_rw_test
printf 'lineA\nlineB\n' > "$TARGET"

# open fd5 for read/write
exec 5<>"$TARGET"

# read first line from fd5
IFS= read -r first_line <&5
printf 'first_line=[%s]\n' "$first_line"

# write at current position (after first line)
printf 'inserted_line\n' >&5

# close and reopen fd5 to reset offset for a full read from start
exec 5<&-
exec 5<>"$TARGET"

# dump file content via fd5
printf 'file content via fd5:\n'
cat <&5

# close fd5
exec 5<&-
