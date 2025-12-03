#!/bin/sh
# complex redirections and pipelines
echo "one two three" | awk '{print $2}' > /tmp/_test_out 2>&1
# append and truncate
printf 'append\n' >> /tmp/_test_out
: > /tmp/_test_out
# here string (bash)
# cat <<< "inline"  # uncomment on bash
