#!/bin/sh
# define and remove aliases (note: aliases are shell-specific and usually expanded in interactive shells)
alias ll='ls -l'
alias gs='git status'

# invoking aliases (may not expand in non-interactive shells, but tokenizer still should recognize the syntax)
# ll /tmp

# remove alias
unalias ll 2>/dev/null || true
