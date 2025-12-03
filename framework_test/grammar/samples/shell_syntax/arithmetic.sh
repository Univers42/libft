#!/bin/sh
# simple arithmetic expansion and assignments
a=$((1 + 2 * 3))
echo "a=$a"

let "b = a + 4"
echo "b=$b"

# nested arithmetic and increment/decrement
c=$(( (a + b) / 2 ))
c=$((c + 1))
echo "c=$c"

# arithmetic in test contexts
if [ $((c % 2)) -eq 0 ]; then
  echo "c is even"
fi
