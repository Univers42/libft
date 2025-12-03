#!/bin/sh
# quote removal and effects on tokenization

# single quotes prevent all expansion and are removed
SINGLE='a $b "c"'
echo 'single-raw:' $SINGLE
echo "single-expanded literal:" '$SINGLE'

# double quotes allow expansions but prevent word-splitting; quotes are removed after expansion
VAL='one two'
echo "double-quoted prevents splitting: \"$VAL\""
for x in "$VAL"; do printf 'dq-item:[%s]\n' "$x"; done

# unquoted expansion is subject to word-splitting and pathname expansion
echo Unquoted-splits:
for x in $VAL; do printf 'uq-item:[%s]\n' "$x"; done

# nested and mixed quoting examples
echo mixed: "pre'$VAL'"'post'

# here-doc quoting: quoted delim prevents expansion (quote removal rules differ)
cat <<'NOEXP'
here-doc literal: $VAL "$(echo hi)"
NOEXP
