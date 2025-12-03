#!/bin/sh
# parameter expansion edge-cases
unset A
echo "A default: ${A:-def}"
B=""
echo "B default assign: ${B:=assigned}"
echo "B after := -> B='$B'"

C=hello_world
echo "prefix removal: ${C#*_}"
echo "greedy prefix removal: ${C##*o}"
echo "suffix removal: ${C%_*}"
echo "substring: ${C:0:5}"

# use of ${var:+alt} and ${var:?err}
echo "C present -> ${C:+yes}"
# uncomment to see error behavior:
# echo "${UNSET:?variable UNSET triggers error}"
