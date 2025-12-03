#!/bin/sh
# backslash escapes and quoting differences
echo "double quotes allow \$expansion and \"quotes\""
echo 'single quotes prevent $expansion and "quotes"'

# escaped newlines and special chars
echo "line1 \
line2"

# ANSI-C quoting (bash/zsh feature): include literal newlines and escapes if supported
# printf $'a\tb\n'  # uncomment on shells supporting $'...' style
