#!/bin/sh
# stdout redirect
echo "this goes to stdout-file" > /tmp/_redir_stdout

# stderr redirect
ls /nonexistent >/dev/null 2>/tmp/_redir_stderr

# combine stdout and stderr into one file
echo "both streams" > /tmp/_both_streams 2>&1
ls /nonexistent >> /tmp/_both_streams 2>&1

# discard stdout and keep stderr
command_missing >/dev/null 2>/tmp/_only_stderr || true

# duplicate fd and use it
exec 3>&1
echo "via fd3 to original stdout" >&3
exec 3>&-
