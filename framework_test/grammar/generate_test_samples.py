#!/usr/bin/env python3
"""
Generate test samples focused on many categories and especially on shell syntax.
Creates files under grammar/samples/*

Usage:
  ./generate_test_samples.py [--out DIR]

The script is idempotent and safe to run multiple times.
"""
import argparse
from pathlib import Path
import stat
import textwrap
import sys

def write(p: Path, content: str, mode_text=False):
    p.parent.mkdir(parents=True, exist_ok=True)
    if not mode_text:
        # normalize line endings and trim trailing spaces
        content = content.replace('\r\n', '\n')
    p.write_text(content, encoding='utf-8')

def make_executable(p: Path):
    st = p.stat()
    p.chmod(st.st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)

def create_shell_samples(out: Path):
    d = out / "shell_syntax"
    samples = {
        "simple.sh": textwrap.dedent("""\
            #!/bin/sh
            # simple script: args, loops, conditional
            echo "Args: $*"
            for a in "$@"; do
              echo "Got: $a"
            done
            if [ -n "$1" ]; then
              echo "First arg exists"
            else
              echo "No args"
            fi
        """),
        "quoting_and_expansions.sh": textwrap.dedent("""\
            #!/bin/sh
            VAR='a b'
            echo "double quoted: \"$VAR\""
            echo 'single quoted: '\''$VAR'\'''
            echo "command substitution: $(echo hi)"
            # parameter expansion edge-cases
            EMPTY=''
            echo "${EMPTY:-default}"
            echo "${UNSET-default}"
            echo "${VAR#* }"
            echo "${VAR##* }"
            echo "${VAR% *}"
            echo "${VAR%% *}"
        """),
        "here_docs.sh": textwrap.dedent("""\
            #!/bin/sh
            cat <<'NOEXP'
            This is a here-doc with 'single-quotes' that prevents expansion: $HOME $(pwd)
            NOEXP

            cat <<EXP
            This one expands: $SHELL
            EXP
        """),
        "functions_and_subshells.sh": textwrap.dedent("""\
            #!/bin/bash
            greet() {
              local name="$1"
              echo "Hello, $name"
            }
            greet "Alice"
            ( echo "in subshell: $$"; ls /nonexistent >/dev/null 2>&1 || echo "ls failed" )
            # capture output of function
            out=$(greet Bob)
            echo "Captured: $out"
        """),
        "arrays_bash.sh": textwrap.dedent("""\
            #!/bin/bash
            # bash-only arrays and associative arrays
            arr=(one two "three four")
            echo "${arr[2]}"
            for v in "${arr[@]}"; do
              printf '[%s]\\n' "$v"
            done

            declare -A map
            map[key1]="value with space"
            map[key2]=42
            for k in "${!map[@]}"; do
              printf "%s => %s\\n" "$k" "${map[$k]}"
            done
        """),
        "redirections_pipelines.sh": textwrap.dedent("""\
            #!/bin/sh
            # complex redirections and pipelines
            echo "one two three" | awk '{print $2}' > /tmp/_test_out 2>&1
            # append and truncate
            printf 'append\\n' >> /tmp/_test_out
            : > /tmp/_test_out
            # here string (bash)
            # cat <<< "inline"  # uncomment on bash
        """),
        "process_substitution.sh": textwrap.dedent("""\
            #!/bin/bash
            # process substitution examples
            diff <(echo a; sleep 0) <(printf 'a')
        """),
        "case_and_test.sh": textwrap.dedent("""\
            #!/bin/sh
            case "$1" in
              start) echo start ;;
              stop) echo stop ;;
              *) echo unknown ;;
            esac

            if test -f /etc/passwd; then
              echo "passwd exists"
            fi
        """),
        "trap_and_signals.sh": textwrap.dedent("""\
            #!/bin/sh
            trap 'echo Trapped; exit 1' INT TERM
            echo "Sleeping for 1 (send SIGINT to test)"
            sleep 1
        """),
        "complex_script.sh": textwrap.dedent("""\
            #!/bin/bash
            # combined features: loops, nested subshells, stderr handling, arithmetic
            for i in $(seq 1 5); do
              (
                val=$((i * i))
                printf "i=%d val=%d\\n" "$i" "$val" 1>&2
                if [ $i -eq 3 ]; then
                  echo "special-$i"
                fi
              )
            done | nl -ba
        """),
        "stress_nested_subshells.sh": "#!/bin/bash\n" + "(\n" + "(".join(["sub"]*80) + " echo nested; " + ")"*80 + "\n",
        "edge_case_ifs.sh": textwrap.dedent("""\
            #!/bin/sh
            # tricky conditionals and quoting
            if [ \"x$VAR\" = \"x\" ]; then
              echo empty
            fi

            # weird eval and printf usage
            cmd='printf \"%s\\n\" \"a b\"'
            eval "$cmd"
        """),
    }
    for name, content in samples.items():
        p = d / name
        write(p, content)
        if name.endswith(".sh"):
            make_executable(p)

def create_maps_samples(out: Path):
    d = out / "maps"
    write(d / "sample.json", textwrap.dedent("""\
        {
          "simple": {"a": 1, "b": [1,2,3]},
          "nested": {"x": {"y": {"z": "deep"}}}
        }
    """))
    write(d / "sample.yaml", textwrap.dedent("""\
        simple:
          - a: 1
          - b: 2
        nested:
          child:
            prop: value
    """))
    write(d / "sample.ini", textwrap.dedent("""\
        [section]
        key=value
        empty=
    """))

def create_strings_formatting(out: Path):
    d = out / "strings_and_formatting"
    # unicode, long lines and printf templates
    write(d / "unicode.txt", "ASCII line\nAccents: é è ü ñ\nEmoji: 😃 🚀\n")
    write(d / "long_line.txt", "A" * 20000 + "\n")
    write(d / "printf_templates.txt", textwrap.dedent("""\
        %04d
        %010u
        %.*s
        %-20s
        %#x
    """))

def create_data_structures(out: Path):
    d = out / "data_structures"
    write(d / "json_array.json", "[{\"id\":1}, {\"id\":2}, {\"id\":3}]")
    write(d / "nested_struct.json", textwrap.dedent("""\
        {
          "users": [
            {"id":1, "roles":["admin","dev"]},
            {"id":2, "roles":[]}
          ],
          "meta": {"count":2}
        }
    """))

def create_databases(out: Path):
    d = out / "databases"
    write(d / "create_tables.sql", textwrap.dedent("""\
        CREATE TABLE users (
          id INTEGER PRIMARY KEY,
          name TEXT NOT NULL,
          bio TEXT
        );
    """))
    write(d / "insert_edgecases.sql", textwrap.dedent("""\
        INSERT INTO users (id, name, bio) VALUES (1, 'Alice', 'Bio with newline\\nand special: %s');
        INSERT INTO users (2, 'BadQuote,', 'unterminated);
    """))

def create_binary_and_misc(out: Path):
    d = out / "binary"
    # small binary with embedded NULs
    p = d / "with_nulls.bin"
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_bytes(b"\x00\x01\x00HELLO\x00WORLD")
    # random-ish (not too large)
    write(d / "hex_bytes.txt", "deadbeef\ncafebabe\n")

def create_stress(out: Path):
    d = out / "stress"
    # huge text file (~1MB) but avoid flooding repo by default making smaller ~100k
    lines = 50000
    p = d / "many_lines.txt"
    p.parent.mkdir(parents=True, exist_ok=True)
    with p.open("w", encoding="utf-8") as f:
        for i in range(lines):
            f.write(f"line-{i:06d}: " + "X"*120 + "\n")
    # large shell script stress
    p2 = d / "long_shell_loop.sh"
    content = "#!/bin/bash\nfor i in $(seq 1 10000); do printf 'item %d\\n' \"$i\"; done\n"
    write(p2, content)
    make_executable(p2)

def create_format_template_variants(out: Path):
    d = out / "format_variants"
    write(d / "c_printf_tests.txt", textwrap.dedent("""\
        %d %i %u %x %X %o %s %p %lld %zu %.*s
        %-5s %05d %#x
    """))

def main():
    parser = argparse.ArgumentParser(description="Generate test samples (focus on shell syntax).")
    parser.add_argument("--out", "-o", default="samples", help="Output directory (relative to this script).")
    args = parser.parse_args()

    base = Path(__file__).resolve().parent
    out = (base / args.out).resolve()
    out.mkdir(parents=True, exist_ok=True)

    create_shell_samples(out)
    create_maps_samples(out)
    create_strings_formatting(out)
    create_data_structures(out)
    create_databases(out)
    create_binary_and_misc(out)
    create_stress(out)
    create_format_template_variants(out)

    # print summary
    print("Created test samples under:", out)
    for p in sorted(out.rglob("*")):
        if p.is_file():
            print("-", p.relative_to(out))

if __name__ == "__main__":
    main()
