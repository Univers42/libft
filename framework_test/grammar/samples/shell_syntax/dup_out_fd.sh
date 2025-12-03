#!/bin/sh
# save current stdout on fd 3, redirect stdout to file, and write to original stdout via fd3

OUT=/tmp/_dup_out_test
# save stdout
exec 3>&1
# redirect stdout to file
exec 1>"$OUT"

echo "this goes to file (stdout redirected)"
# write directly to saved original stdout
echo "this goes to original stdout via fd3" >&3

# restore stdout from fd3 and close fd3
exec 1>&3
exec 3>&-

echo "file content:"
cat "$OUT"
