# Glob lib
This lib contain directories and files that specify globbing and pattern matching subsystem. 

> globbing referes to filename pattern matching using wildcards like `*`, `?`, and bracket expresions [abc]

# Features
- Standard globbing: Wildcars `*`, `?`), bracket expressions
- Extended globbing: ksh-like patterns @(foo|bar), !foo, +(foo), etc..
- Multibyte/Unicode support: Handles wilde character
- POSIX compliance: supports POSIX bracket expresssion like [[:alpha:]], [.newline.],[=a=]
- Case folding: Optional case-insensitive matching.
- Directory-specific matching: Flags to match only directories, mark directories, etc..
- Recursive globiing: support for `**` to match files recursively

# HOW IT WORKS
The globbing engine scans directories, matches filenames against patterns using the matching engin and return lists
of matches

# Why it's a lib ?
it implements filename pattern matching (globbing) using wildcard and extended patterns, which is  acommon need in shell
and file utilisies.
- The code is modular and portable, so it can be reused in any other porjets of (shells, scripting langues, file managers)
that need globbing at the end of the day. 

> It's adaptability make it adaptable and completeness make it useful for other porjects beyond shell !c

# COLLSYMS.h
- [x] collsym
- [x] posixcoll

# glob.c

- [ ]glob_pattern
- [ ]extglob_skipname
- [ ]skipname
- [ ]udequote_pathname
- [ ]glob test_dir
- [ ]finddirs
- [ ]glob_vector
- [ ]glob_dir_to_array
- []glob_filename

# glob_loop.c
- [ ]Internal glob pattern p

# gm_loop.c
- [ ]EXTGLOB_PATTERN_P
- [ ]MATCH_PATTER_CHAR
- [ ]MATCHLEN
- [ ]
- [ ]
- [ ]
- [ ]

# gmisc.c
- [ ]glob_dirscsan


# sm_loop.c
- [ ]FCT
- [ ]GMATCH
- [ ]SLASH_PATHNAME
- [ ]PARSE_SUBBRACET
- [ ]BRACKMATCH
- [ ]PATSCAN
- [ ]STRCOMPARE
- [ ]EXTMATCH
- [ ]

# smatch.c
- [ ]fnmatch_fallback
- [ ]fnmatch
- [ ]charcmp
- [ ]rangecmp
- [ ]collseqcmp
- [ ]collsym
- [ ]char_class
- [ ]is_valid_cclass
- [ ]cclass_test
- [ ]is_cclass
- [ ] FOLD
- [ ] ISDIRSEP
- [ ] PATHSEP
- [ ] PDOT_OT_DOTDOT
- [ ] SDOT_OR_DOTDOT
- [ ] FNMATCH_EQUIV_FALLBACK
- [ ] fnmatch_fallback_wc
- [ ] charcmp_wc
- [ ] rangecmp_wc
- [ ] collseqcmp_wc
- [ ] collwcsym
- [ ] is_wcclass
- [ ] posix_cclass_only
## map of fns
```c
#define FCT			internal_wstrmatch
#define GMATCH			gmatch_wc
#define COLLSYM			collwcsym
#define PARSE_SUBBRACKET	parse_subbracket_wc
#define BRACKMATCH		brackmatch_wc
#define PATSCAN			glob_patscan_wc
#define STRCOMPARE		wscompare
#define EXTMATCH		extmatch_wc
#define DEQUOTE_PATHNAME	wcdequote_pathname
#define STRUCT			wcsmat_struct
#define STRCHR(S, C)		wcschr((S), (C))
#define MEMCHR(S, C, N)		wmemchr((S), (C), (N))
#define STRCOLL(S1, S2)		wcscoll((S1), (S2))
#define ft_strlen(S)		wcslen(S)
#define STRCMP(S1, S2)		wcscmp((S1), (S2))
#define RANGECMP(C1, C2, F)	rangecmp_wc((C1), (C2), (F))
#define COLLEQUIV(C1, C2)	collseqcmp_wc((C1), (C2))
#define CTYPE_T			enum char_class
#define IS_CCLASS(C, S)		is_wcclass((C), (S))
#include "sm_loop.c"
```
- [ ] xstrmatch

# strmatch.c
- [ ] strmatch
- [ ] wcstrmatch

# xmbstrtowcs.h
- [ ] xmbsrtowcs
- [ ] xdumpstowcs2
- [ ] xdupmbstowcs
- [ ] xwcsrtombs


 # TEST CASES
 short checklist of edge cases to exercices our glob implementation (example pattern behaviour / what to verify)
 
 ## basic wildcar
 - *.c --> matches filles ending in .c only (not directories unless names end with .c)
 - ? ->single-char match: a?.txt matches a1.txt but not a12.txt
 ## Multiple/consecurtive stars
 - `**` -> **treated like a single; test`. `.c` and `.c` produce the same list
 ## Character classes and negation
 [abc].txt, [!a]*.log → correct selection and negation semantics
Ranges: [0-9].dat and malformed ranges (e.g. [z-a]) → behaviour / error handling
 ## leading dot handling
 *.txt must NOT match ".secret.txt"
.*.txt must match dotfiles
Patterns with segment-internal leading dot: in path "dir/.foo", matching depending on segment-first flag
 ## path separators and segments
 a/*/b.txt matches only when there's exactly one segment between a and b.txt
a/**/b (if you support recursive glob) — otherwise ensure expected non-match
"/abs/path/.c" vs "relative/.c"
 ## Tailing slash / directory-only matches
 pattern ending with '/' or pattern that should only match directories: ensure directories vs files distinction

 ## No match behavior
 pattern that matches nothing: ensure you return either the original word (no-match policy) or empty list, per spec; test both cases
 ## Sorting duplicates
 Create files in random name order; verify result is sorted and unique (if expected)
Duplicates from different branches should be either deduped or preserved consistently
 ## hidden/parent entries
 "." and ".." should not be matched by wildcards unless explicitly requested
Patterns that could match "." or ".." (e.g., single-char patterns) — verify rejection
 ## quotes /  non expnadable rejection
 Patterns inside single quotes: '*.c' (quoted) should be literal, not expanded
Double-quoted behavior and escaping: "a*.c" vs a*.c
 ## escapes and backslahes
 a*b should match literal "a*b" not act as wildcard
backslash escaping inside double quotes vs single quotes
 ##  environment / variable interplay
 $HOME/*.txt (if expansion done before glob) vs literal $HOME — verify order of expansion relative to glob
 ## character encoding & special bytes
 Filenames with spaces, Unicode, high-bit bytes — ensure matching and sorting behave correctly
 ## symlinks and fily type
 Symlink to file vs symlink to dir — ensure match behavior consistent and not following links unless intended
 ## File permissions & undereabl dirs
 Directory traversal where readdir fails (permission denied) — check handling and error propagation
## REcursion depth and unwind / signals
If the implementation uses recursion or a global unwind flag (get_g_sig()->should_unwind), simulate interruption (signal or set flag) and ensure it aborts cleanly without leaks
## Edge lexing interactions (tokenization effects)
Patterns that include tokens treated specially by tokenizer (dollar, quotes, braces) to ensure tokenization stage marks them as non-expandable when appropriate