Grammar test inputs — categories and generator

Purpose:

- A categorized box of input-only files to exercise parsers, I/O, formatting, and edge cases.
- Files are inputs only (no expected results). Use these to feed programs or unit tests.

Categories created by the generator:

- strings : empty, long lines, lines with only whitespace, control chars
- unicode : UTF-8 samples, BOM, multi-byte characters
- numbers : integers, unsigned, overflow-like strings
- json : well-formed, deeply nested, and malformed JSON
- csv : normal CSV, quoted edge cases, malformed records
- filenames : filenames with special characters, very long names, non-printable names (listed)
- format : format string inputs and corner cases
- binary : files with embedded NULs and random bytes
- stress : large files for streaming/performance (1MiB+)

How to generate:

- cd .../framework_test/grammar
- chmod +x create_samples.sh
- ./create_samples.sh

Notes:

- The generator uses standard POSIX utilities and Python3 if available.
- Edit create_samples.sh to customize content, sizes, or add categories.
