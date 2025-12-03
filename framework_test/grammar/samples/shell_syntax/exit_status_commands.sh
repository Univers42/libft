#!/bin/sh
# examples of capturing exit statuses

true
echo "true -> $?"

false
echo "false -> $?"

# command substitution exit status is that of the substitution's last command
$(false) 2>/dev/null || true
echo "command substitution exit (ignored earlier): $?"

# background job and wait
sleep 0.01 &
bgpid=$!
wait "$bgpid"
echo "waited on $bgpid -> $?"

# pipeline exit status: POSIX shells return status of last command by default
false | true
echo "pipeline status (last cmd): $?"
