#!/usr/bin/env bash
set -euo pipefail

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT="$BASE_DIR/samples"
mkdir -p "$OUT"

echo "Generating grammar test samples in $OUT ..."

# strings category
mkdir -p "$OUT/strings"
printf "Hello\nWorld\n" > "$OUT/strings/text_short.txt"
# very long single line
python3 - <<'PY' > "$OUT/strings/long_line.txt"
print("A" * 10000)
PY
# whitespace and control chars
printf " \t\n\n\n" > "$OUT/strings/only_whitespace.txt"
printf "line1\nline2\r\nline3\n\0embedded_null\n" > "$OUT/strings/control_chars.txt"

# unicode category
mkdir -p "$OUT/unicode"
cat > "$OUT/unicode/utf8.txt" <<'EOF'
ASCII line
Accents: é è ê ü å ö
Emoji: 😃 🚀 🐍
Chinese: 中文测试
Symbols: ☃ ♞ ✓ ≈ ∑
EOF
# UTF-8 BOM file
printf '\xEF\xBB\xBF' > "$OUT/unicode/utf8_bom.txt"
printf "BOM line\n" >> "$OUT/unicode/utf8_bom.txt"

# numbers category
mkdir -p "$OUT/numbers"
cat > "$OUT/numbers/ints.txt" <<'EOF'
0
1
-1
2147483647
-2147483648
9223372036854775807
-9223372036854775808
99999999999999999999999999
EOF

cat > "$OUT/numbers/hex_and_octal.txt" <<'EOF'
0x1a
0XDEADBEEF
0755
invalid123
EOF

# json category
mkdir -p "$OUT/json"
cat > "$OUT/json/well_formed.json" <<'EOF'
{
  "name": "example",
  "value": 123,
  "array": [1, 2, 3],
  "nested": { "a": true }
}
EOF

# deep nested JSON (use python if available)
if command -v python3 >/dev/null 2>&1; then
python3 - <<'PY' > "$OUT/json/deep.json"
depth = 200
obj = {}
cur = obj
for i in range(depth):
    cur["n"] = {}
    cur = cur["n"]
cur["end"] = "ok"
import json, sys
json.dump(obj, sys.stdout)
PY
else
printf '{"n":' > "$OUT/json/deep.json"
for i in $(seq 1 200); do printf '{"n":' >> "$OUT/json/deep.json"; done
printf '"end"' >> "$OUT/json/deep.json"
for i in $(seq 1 200); do printf '}' >> "$OUT/json/deep.json"; done
fi

cat > "$OUT/json/malformed.json" <<'EOF'
{ "a": 1, "b": [1,2, 3,],  "c": "unterminated }
EOF

# csv category
mkdir -p "$OUT/csv"
cat > "$OUT/csv/normal.csv" <<'EOF'
id,name,age
1,Alice,30
2,Bob,25
EOF

cat > "$OUT/csv/malformed.csv" <<'EOF'
id,name,age
1,"Unclosed quote,30
2,Normal,20
"3","Quoted, with, commas",40
EOF

cat > "$OUT/csv/edge_cases.csv" <<'EOF'
"","empty field",,
"quote""inside","line\nbreak",123
EOF

# filenames category (list of example names to create later if needed)
mkdir -p "$OUT/filenames"
cat > "$OUT/filenames/list.txt" <<'EOF'
normal.txt
space in name.txt
trailing-space- .txt
unicode-文件.txt
very_long_name_$(printf 'x%.0s' {1..200}).txt
name_with_newline
null_char_name (can't be created easily in FS)
EOF

# format category (format-string-like inputs)
mkdir -p "$OUT/format"
cat > "$OUT/format/printf_like.txt" <<'EOF'
%04d
%010u
%#x
%lld
%0*d
%.*s
EOF

# binary category
mkdir -p "$OUT/binary"
# binary with embedded NULs and patterns (using printf)
printf '\x00\x01\x00\x02\x00\x03HELLO\x00WORLD' > "$OUT/binary/with_nulls.bin"
# random bytes if /dev/urandom available
if [ -r /dev/urandom ]; then
    head -c 1024 /dev/urandom > "$OUT/binary/random_1k.bin"
fi

# stress category (larger files)
mkdir -p "$OUT/stress"
# 1MiB zero-filled file
if command -v dd >/dev/null 2>&1; then
    dd if=/dev/zero of="$OUT/stress/zeros_1MiB.bin" bs=1M count=1 status=none || true
else
    head -c 1048576 </dev/zero > "$OUT/stress/zeros_1MiB.bin" || true
fi

# very long repeated pattern text (100k lines)
python3 - <<'PY' > "$OUT/stress/huge_lines.txt"
for i in range(100000):
    print("line-%08d: " % i + "X"*80)
PY

echo "Done. Sample files created under $OUT (categories: strings, unicode, numbers, json, csv, filenames, format, binary, stress)."
ls -lh "$OUT" || true
