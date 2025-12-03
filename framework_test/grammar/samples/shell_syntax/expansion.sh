#!/bin/sh
# command substitution (modern and legacy)
now=$(date +%s)
echo "now=$now"
legacy=`echo legacy-subst`
echo "$legacy"

# arithmetic expansion inside other expansions
num=$((10 + 5))
echo "num=${num}"

# parameter expansion variants
X=""
echo "default: ${X:-default}"
Y=hello
echo "length of Y: ${#Y}"
echo "replace: ${Y/l/1}"
