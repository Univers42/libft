-# POSIX Shell Grammar Quick Reference

## 1. Lexical Conventions

- **Tokenization order**: detect multi-char operators ➜ detect `IO_NUMBER` (digits before `<`/`>`) ➜ treat remainder as `TOKEN`.
- **Context-sensitive TOKENs** convert to `WORD`, `NAME`, `ASSIGNMENT_WORD`, or reserved words using Rules 1‑9.
- **Word expansion** occurs at execution time, never during parsing (except here-doc delimiter quote removal).
- **Escaped newlines** are removed before tokenization; quoted strings keep their quotes until after reserved-word checks.

## 2. Rule Summary

1. Default: TOKEN → WORD (unless other rules override).
2. `filename` positions: apply redirection expansions; must yield exactly one field.
3. `here_end`: apply quote removal to determine literal delimiter.
4. `case` patterns: first WORD applies rule 4 (no expansion) but alternation words bypass rule 4.
5. `for` variable name must be `NAME`.
6. Third word of `for`/`case`: recognize `in` (and `do` for `for`) when correctly positioned.
7. Assignment handling
   - 7a: first word of simple command → `cmd_name` (reserved-word check).
   - 7b: assignments preceding command name.
8. Function name: reserved words take precedence; otherwise require `NAME`.
9. Function body: suppress word/assignment expansion while parsing compound command and redirects.

## 3. Token Inventory

| Token Type          | Examples                                                                         |
| ------------------- | -------------------------------------------------------------------------------- | --- | -------------------------------------------------------------------- | --- |
| Operators           | `&&` (`AND_IF`), `                                                               |     | ` (`OR_IF`), `;;` (`DSEMI`), `<<`, `>>`, `<&`, `>&`, `<>`, `<<-`, `> | `   |
| Redirection helpers | `<`, `>`, `IO_NUMBER<n>` + `io_file`                                             |
| Reserved words      | `if then else elif fi`, `do done`, `case esac`, `while until for`, `{ } !`, `in` |
| Structural tokens   | `(` `)` for subshells, newline (`NEWLINE`), separators `;` and `&`               |

## 4. Grammar Skeleton

```
program → linebreak complete_commands? linebreak
complete_command → list [separator_op]
list → and_or { separator_op and_or }
and_or → pipeline { (&& | ||) linebreak pipeline }
pipeline → [!] pipe_sequence
pipe_sequence → command { '|' linebreak command }
command → simple_command
        | compound_command [redirect_list]
        | function_definition
```

Compound commands include `{}` groups, subshells, `for`, `case`, `if`, `while`, `until`. `simple_command` splits into `cmd_prefix` (assignments/redirections), `cmd_name`, `cmd_suffix` (args/redirections). `fname () compound-command [io-redirect]` defines functions; `function_body` parsing obeys Rule 9.

## 5. Execution Notes

- Assignments in `cmd_prefix` apply to the command environment.
- Function invocation replaces positional parameters (`$1`, `$#`) but leaves `$0`.
- Here-doc delimiters respect quoting rules (Rule 3) and honor `<<-` tab stripping.
- `separator_op` (`;` or `&`) determines sequential vs asynchronous execution; `newline_list` also separates commands.
- `do_group` and `case` arms require `done`/`esac` reserved words recognized via contextual Rules 5/6.

Use this file as a companion to the executable samples in this directory when testing tokenizer and parser compliance with POSIX §2.10.
