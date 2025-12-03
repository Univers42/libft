#!/bin/sh
# special parameters demonstration
sleep 0.01 &
bgpid=$!
echo "background pid: $bgpid (via \$!)"
wait $bgpid
echo "last status: $?"

echo "shell pid: $$"
echo "arg count: $#"
echo "last arg: \$_=$_"
echo "current flags: \$-=$-"
echo "script name: \$0=$0"
