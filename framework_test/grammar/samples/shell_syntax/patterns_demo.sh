#!/bin/sh
# POSIX pattern and filename expansion demonstration (2.13.*)

set -u

say() { printf '\n=== %s ===\n' "$1"; }

TMPDIR=${TMPDIR:-/tmp}/sh_pattern_demo_$$
mkdir -p "$TMPDIR/subdir" "$TMPDIR/dotdir"
trap 'rm -rf "$TMPDIR"' EXIT INT TERM

# Prepare some files for pathname expansion tests
: >"$TMPDIR/a"
: >"$TMPDIR/ab"
: >"$TMPDIR/abc"
: >"$TMPDIR/x1"
: >"$TMPDIR/x2"
: >"$TMPDIR/x_long"
: >"$TMPDIR/a[b/c]d"
: >"$TMPDIR/subdir/file"
: >"$TMPDIR/subdir/other"
: >"$TMPDIR/.hidden"
: >"$TMPDIR/.hx"
: >"$TMPDIR/visible"
: >"$TMPDIR/h-1"
: >"$TMPDIR/h-9"

###############################################################################
# 2.13.1 Patterns Matching a Single Character
###############################################################################
say "2.13.1: ordinary characters match themselves"

cd "$TMPDIR" || exit 1

# ordinary characters
printf 'pattern "a" against names: %s\n' a
set -- a
printf 'matched: [%s]\n' "$@"

say "2.13.1: question mark (?) matches any single character"

set -- a?c    # pattern only; show intent
printf 'pattern a?c (no glob yet, just literal): [%s]\n' "$1"
# create matches explicitly
: >"$TMPDIR/abc"
: >"$TMPDIR/axc"
cd "$TMPDIR" || exit 1
set -- a?c
printf 'expanded a?c ->'
printf ' [%s]' "$@"
printf '\n'

say "2.13.1: bracket expressions [...] and non-matching lists [!]"

# characters x1, x2, x_long exist in TMPDIR
set -- x[12]
printf 'pattern x[12] ->'
printf ' [%s]' "$@"
printf '\n'

set -- x[!12]
printf 'pattern x[!12] ->'
printf ' [%s]' "$@"
printf '\n'

# range expression
set -- h-[1-5]
printf 'pattern h-[1-5] ->'
printf ' [%s]' "$@"
printf '\n'

say "2.13.1: escaping special pattern chars with backslash (\\)"

# literals: \?, \*, \[, \\
: >"$TMPDIR/q?file"
: >"$TMPDIR/star*file"
: >"$TMPDIR/bracket[file"
: >"$TMPDIR/back\\slash"
set -- q\?file
printf 'pattern q\?file matches literal "q?file":'
printf ' [%s]\n' "$@"

set -- star\*file
printf 'pattern star\*file matches literal "star*file":'
printf ' [%s]\n' "$@"

set -- bracket\[file
printf 'pattern bracket\[file matches literal "bracket[file":'
printf ' [%s]\n' "$@"

set -- back\\slash
printf 'pattern back\\slash matches literal "back\slash":'
printf ' [%s]\n' "$@"

say "2.13.1: backslash at end of pattern is unspecified; show literal"

# We won't rely on behavior; just show literal string
printf 'literal with trailing backslash in echo: "foo\\" (no globbing context)\n'

###############################################################################
# 2.13.2 Patterns Matching Multiple Characters
###############################################################################
say "2.13.2: asterisk (*) matches any string (including empty)"

cd "$TMPDIR" || exit 1

set -- *
printf 'pattern * ->'
printf ' [%s]' "$@"
printf '\n'

# demonstrate that * can match empty suffix
: >"$TMPDIR/prefix"
: >"$TMPDIR/prefixX"
set -- prefix*
printf 'pattern prefix* ->'
printf ' [%s]' "$@"
printf '\n'

say "2.13.2: concatenation of single-character patterns"

# a? matches a single char suffix
: >"$TMPDIR/pa1"
: >"$TMPDIR/pa9"
: >"$TMPDIR/pa99"
set -- pa?
printf 'pattern pa? (single char after pa) ->'
printf ' [%s]' "$@"
printf '\n'

say "2.13.2: concatenation of single-char patterns with * (greedy *)"

: >"$TMPDIR/file_ab_cd"
: >"$TMPDIR/file_ac_cd"
: >"$TMPDIR/file_ax_cd"
set -- file_a*cd
printf 'pattern file_a*cd (greedy *) ->'
printf ' [%s]' "$@"
printf '\n'

###############################################################################
# 2.13.3 Patterns Used for Filename Expansion
###############################################################################
say "2.13.3: slash (/) is never matched by ?, *, or []"

cd "$TMPDIR" || exit 1

printf 'Have subdir/ with files: '
ls subdir 2>/dev/null || true

# *file matches in current directory, not across subdir/
set -- *file
printf 'pattern *file in $TMPDIR ->'
printf ' [%s]' "$@"
printf '\n'

cd "$TMPDIR/subdir" || exit 1
set -- *file
printf 'pattern *file in $TMPDIR/subdir ->'
printf ' [%s]' "$@"
printf '\n'

cd "$TMPDIR" || exit 1

say "2.13.3: slash inside [ ] stops bracket expression for globbing"

# a[b/c]d was created as literal name
set -- a[b/c]d
printf 'pattern a[b/c]d literally matches "a[b/c]d":'
printf ' [%s]\n' "$@"

say "2.13.3: leading dot (.) is not matched by * or ?"

cd "$TMPDIR" || exit 1

printf 'Existing dot entries: '
ls -d .?* 2>/dev/null || printf '(none)\n'

set -- *
printf 'pattern * (no dotfiles) ->'
printf ' [%s]' "$@"
printf '\n'

set -- ?
printf 'pattern ? (no dotfiles) ->'
printf ' [%s]' "$@"
printf '\n'

say "2.13.3: explicit dot must be used to match leading dot"

set -- .*
printf 'pattern .* includes leading-dot files ->'
printf ' [%s]' "$@"
printf '\n'

set -- .h*
printf 'pattern .h* ->'
printf ' [%s]' "$@"
printf '\n'

say "2.13.3: non-matching list [!a] does NOT match leading dot"

# .hidden and .hx exist; [!a] cannot be used to match the leading '.' (per spec)
set -- [!a]*
printf 'pattern [!a]* (should not match .hidden/.hx as leading dot) ->'
printf ' [%s]' "$@"
printf '\n'

say "2.13.3: zero matches leave pattern unchanged"

cd "$TMPDIR" || exit 1

# Use a pattern unlikely to match anything
set -- nomatch_foo*
printf 'pattern nomatch_foo* (no existing path) ->'
printf ' [%s]\n' "$@"

say "2.13.3: pattern components and permissions note (demonstration)"

printf 'Pattern "%s" would require search+read perms on each directory with globs.\n' "$TMPDIR"/x*/file
printf 'We only demonstrate syntax here; no permission changes performed.\n'

say "Summary: this script exercised all single- and multi-character patterns and filename expansion rules"
