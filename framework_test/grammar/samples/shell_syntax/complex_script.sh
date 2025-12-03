#!/bin/bash
# combined features: loops, nested subshells, stderr handling, arithmetic
for i in $(seq 1 5); do
  (
    val=$((i * i))
    printf "i=%d val=%d\n" "$i" "$val" 1>&2
    if [ $i -eq 3 ]; then
      echo "special-$i"
    fi
  )
done | nl -ba
