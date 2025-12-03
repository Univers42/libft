# Factory of test

instead of scattering the files field, or strings or any boolean all over the tests..
I've decided to centralize everythign in one place..

This gives us:
- consistent data
- readability
- reuse
- easier debugging
- easier extension
- "single source of truth"

> note: I've created that because I was seeing myself struggling too much wiht the edge cases. I was seeing myself repeating
the same and same errors each time.. it was time to do something... So I've decided to create this directory that recompile
all kind of tests I shalle need for my project. it's exhaustive enough for my libft and of course easily extensible so I won't worry
anymore to add just one little case. It has been thought like a playlist for our test inputs


# Static test files
- Deterministic easy to review in PRs
- don't change unless we want them to
- we see the data immediately in Github
- zero "generator code" to maintain
- editors can syntax higlight them

# Generated test Datas (via class namespace)
- useful when the file must be extremly large (MB/GB)
- it must change depending on the case
- we want hundreds of variations
- we're doing randomized fuzzing
- we generate pathological patterns

So basically it's great for:
- stress tests
- fuzz tests
- "infinite combinations" tests
but it's harder this way...

Purpose:
- A central place to keep or generate example files that exercise edge cases for programs.
- Use these files to test file parsing, encoding handling, binary input, streaming, CLI tools, etc.

Quick start:
- Run the generator to create the sample files:
  ./create_edgecases.sh

Contents created by the script:
- samples/text_short.txt        : small plain text
- samples/text_long_line.txt    : a single very long line
- samples/unicode.txt           : various Unicode characters (UTF-8)
- samples/utf16le.txt           : UTF-16 LE encoded text
- samples/binary_with_nulls.bin : small binary containing NULs
- samples/random.bin            : random bytes
- samples/huge_file.bin         : 1 MiB zero-filled file (useful for streaming tests)
- samples/empty_file.txt        : empty file
- list.txt                      : descriptions of files

Notes:
- The generator uses standard POSIX utilities and Python3 if available.
- You can add more files manually into the samples/ directory as you find new edge cases.
