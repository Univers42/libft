#!/usr/bin/env bash
# star_pattern_tricks.sh — demonstrate glob/star edge cases and useful toggles

set -euo pipefail

workdir="$(mktemp -d)"
cd "$workdir"

# create files that exercise tricky names
touch alpha.txt alphabet a_star.txt "a*star.txt" "data?.txt" "file[1].txt" ".hidden" normal.sh "spaces in name.txt" "multi.ext.tar.gz" "file-123" bbb
mkdir -p dir/subdir
touch dir/subdir/test.md dir/README.md

ech() { printf '\n== %s ==\n' "$1"; }

ech "working dir: $workdir"
ls -la

ech "Basic '*' (does not match leading-dot files by default)"
echo *

ech "Explicit dotfiles via '.*'"
echo .*

ech "Using dotglob to include dotfiles"
shopt -s dotglob
echo *
shopt -u dotglob

ech "Matching literal '*' in filenames (escape or quote)"
echo "pattern a*star.txt ->"
echo a*star.txt
echo "escaped a\\*star.txt ->"
echo a\*star.txt
echo "quoted 'a*star.txt' ->"
echo 'a*star.txt'

ech "'?' matches a single character"
echo data?.txt

ech "Character classes and ranges"
echo file[1].txt
echo file[1-3].txt

ech "Negation [!a]* (exclude names starting with 'a')"
echo [!a]*

ech "Files with spaces: unquoted vs safe printing"
echo "Unquoted * (words may be split in further processing):"
echo *
echo "Safe one-per-line:"
for f in *; do printf '%s\n' "$f"; done

ech "Preventing glob expansion: disable globbing (set -f)"
set -f
echo "* literal via set -f:"
echo *
set +f

ech "nullglob: unmatched patterns expand to nothing"
echo "Without nullglob:"
echo *.nomatch
shopt -s nullglob
echo "With nullglob:"
echo *.nomatch
shopt -u nullglob

ech "failglob: unmatched patterns cause error (demonstrate in subshell)"
if ( shopt -s failglob; echo "try failglob: " *.nomatch ) 2>/dev/null; then
  echo "No failglob error (unexpected)"
else
  echo "failglob triggered an error for unmatched pattern (expected)"
fi

ech "extglob for advanced patterns"
shopt -s extglob
echo @(alpha*|bbb)
shopt -u extglob

ech "globstar for recursive matching (bash >=4)"
if shopt -q globstar 2>/dev/null; then
  shopt -s globstar
  echo "**/*.md ->"
  echo **/*.md
  shopt -u globstar
else
  echo "globstar not available in this bash"
fi

ech "Arrays to detect empty matches safely"
shopt -s nullglob
arr=( *.nomatch )
printf 'array length: %s\n' "${#arr[@]}"
shopt -u nullglob

ech "Brace expansion happens before globbing: {a,b}*.txt ->"
echo {a,b}*.txt

ech "Quoted representations for debugging:"
for f in *; do printf '%q\n' "$f"; done

ech "Cleanup"
cd /
rm -rf "$workdir"
echo "removed $workdir"