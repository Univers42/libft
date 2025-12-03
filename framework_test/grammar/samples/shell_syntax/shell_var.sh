#!/bin/sh
# export and readonly variable examples
FOO=bar
export FOO
readonly IMMUTABLE=const

echo "FOO=$FOO"
# IMMUTABLE=other  # would fail in shells that enforce readonly

# demonstrate environment inheritance for subprocess
sh -c 'echo "in subshell, FOO=$FOO"'
