#!/bin/sh
cat <<'NOEXP'
This is a here-doc with 'single-quotes' that prevents expansion: $HOME $(pwd)
NOEXP

cat <<EXP
This one expands: $SHELL
EXP

# here-doc with leading-tab stripping (<<-), useful for indented scripts
# lines prefixed with a tab are stripped
cat <<-TABSTRIP
	Line one with leading tab removed
	Line two with leading tab removed: $SHELL
TABSTRIP

# capture here-doc into a variable (command substitution)
HERED_VAR=$(cat <<EOF
captured line 1
captured var expands: ${SHELL:-/bin/sh}
EOF
)
printf 'HERED_VAR=[%s]\n' "$HERED_VAR"

# redirect a here-doc directly into a file
cat > /tmp/_here_doc_file <<EOF
This content is written to a file via here-doc.
EOF
