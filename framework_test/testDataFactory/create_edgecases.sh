#!/usr/bin/env bash
set -euo pipefail

OUTDIR="samples"
mkdir -p "$OUTDIR"

echo "Generating edge-case sample files in $OUTDIR/ ..."

# small plain text
printf "Hello, EdgeCases!\nThis is a short text file.\n" > "$OUTDIR/text_short.txt"

# very long single line (10k chars)
# use awk/perl/python depending on availability
if command -v perl >/dev/null 2>&1; then
    perl -e 'print "A" x 10000' > "$OUTDIR/text_long_line.txt"
    printf "\n" >> "$OUTDIR/text_long_line.txt"
elif command -v python3 >/dev/null 2>&1; then
    python3 - <<'PY' > "$OUTDIR/text_long_line.txt"
print("A"*10000)
PY
else
    # fallback: repeat many times
    for i in $(seq 1 200); do printf 'AAAAAAAAAA'; done > "$OUTDIR/text_long_line.txt"
    printf "\n" >> "$OUTDIR/text_long_line.txt"
fi

# unicode UTF-8 text
cat > "$OUTDIR/unicode.txt" <<'EOF'
ASCII line
Accents: é è ê ü å ö
Emoji: 😃 🚀 🐍
Symbols: ☃ ♞ ✓ ≈ ∑
EOF

# UTF-16 LE with BOM using python3 if available
if command -v python3 >/dev/null 2>&1; then
    python3 - <<'PY' > "$OUTDIR/utf16le.txt"
text = "This is UTF-16 LE with BOM.\nAccents: é ü ñ\nEmoji: 😃\n"
open("UTF16_TMP", "wb").write(text.encode("utf-16-le"))
# prepend BOM for UTF-16 LE
with open("UTF16_TMP", "rb") as r, open("$OUTDIR/utf16le.txt", "wb") as w:
    w.write(b'\xff\xfe')
    w.write(r.read())
import os
os.remove("UTF16_TMP")
PY
else
    # create a small binary with BOM + UTF-16 LE content using printf (approx)
    printf '\xff\xfe' > "$OUTDIR/utf16le.txt"
    printf 'T\0h\0i\0s\0 \0U\0T\0F\0-\016\0 \0\n\0' >> "$OUTDIR/utf16le.txt" || true
fi

# binary with embedded NULLs
printf '\0\1\0\2\0\3\0\4' > "$OUTDIR/binary_with_nulls.bin"

# random bytes (1 KiB)
if [ -r /dev/urandom ]; then
    head -c 1024 /dev/urandom > "$OUTDIR/random.bin"
else
    # fallback to repeating a pattern
    for i in $(seq 1 128); do printf '%s' "$(printf '\\x%02x' $((i%256)))"; done > "$OUTDIR/random.bin"
fi

# huge file (1 MiB of zeros) — useful for streaming/perf tests
dd if=/dev/zero of="$OUTDIR/huge_file.bin" bs=1M count=1 status=none || \
(head -c 1048576 </dev/zero > "$OUTDIR/huge_file.bin") || true

# empty file
: > "$OUTDIR/empty_file.txt"

echo "Done. Files created in $OUTDIR/:"
ls -lh "$OUTDIR" || true