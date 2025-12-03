#!/bin/sh
# brace expansion (bash/zsh) and globbing
# echo file{1,2}.txt   # uncomment on shells that support brace expansion

# globbing
touch /tmp/_sample_glob_a /tmp/_sample_glob_b 2>/dev/null || true
echo /tmp/_sample_glob_*

# IFS and word splitting differences
IFS=:
LIST="one:two:three"
for item in $LIST; do
  echo "item=$item"
done

# quoting prevents splitting
LIST2="one two three"
for item in "$LIST2"; do
  echo "quoted-item=$item"
done
