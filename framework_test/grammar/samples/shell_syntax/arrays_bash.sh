#!/bin/bash
# bash-only arrays and associative arrays
arr=(one two "three four")
echo "${arr[2]}"
for v in "${arr[@]}"; do
  printf '[%s]\n' "$v"
done

declare -A map
map[key1]="value with space"
map[key2]=42
for k in "${!map[@]}"; do
  printf "%s => %s\n" "$k" "${map[$k]}"
done
