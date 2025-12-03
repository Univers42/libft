#!/bin/sh
VAR='a b'
echo "double quoted: "$VAR""
echo 'single quoted: '''$VAR''''
echo "command substitution: $(echo hi)"
# parameter expansion edge-cases
EMPTY=''
echo "${EMPTY:-default}"
echo "${UNSET-default}"
echo "${VAR#* }"
echo "${VAR##* }"
echo "${VAR% *}"
echo "${VAR%% *}"
