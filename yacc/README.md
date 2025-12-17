# Lexer/Tokenizer Documentation

## Overview

This lexer provides tokenization for shell commands with support for:

- Pipes and redirections
- Logical operators (&&, ||)
- Quotes (single, double, backtick)
- Variable expansion
- Globbing patterns
- Heredocs
- Parameter expansion

## Architecture

### Main Components

1. **tokenizer.c** - Main tokenization interface (simple, robust)
2. **scanner.c** - Low-level scanning with dispatch table
3. **handlers_table.c** - Token handlers for each character type
4. **map_tokens.c** - Token type to string mapping
5. **debug_token.c** - Enhanced debugging utilities

### Token Structure

```c
typedef struct s_token {
    t_token_type    type;       // Token type enum
    const char      *start;     // Pointer to start in input
    int             len;        // Length of token
    int             line;       // Line number
    bool            allocated;  // Whether memory is allocated
} t_token;
```

## Usage

### Basic Tokenization

```c
#include "lexer.h"
#include "ds.h"

t_deque tokens;
deque_init(&tokens, sizeof(t_token));

char *input = "ls -la | grep test";
char *prompt = tokenizer(input, &tokens);

if (prompt) {
    // Incomplete input, need more (e.g., unclosed quote)
    printf("Continue with: %s\n", prompt);
} else {
    // Process tokens
    print_tokens(&tokens);
}

deque_destroy(&tokens);
```

### Testing

```bash
# Run test suite
./test_liblex --test

# Interactive mode
./test_liblex --interactive

# Single command
./test_liblex "echo hello world"
```

## Token Types

See `lexer.h` for complete list. Major categories:

- **CAT_SYS_OP** - System operators (|, &&, ;)
- **CAT_REDIR_IO** - Redirections (<, >, >>)
- **CAT_KEYWORDS** - Shell keywords (if, while, for)
- **CAT_LITERALS** - Words and identifiers
- **CAT_SPECIAL_PARAMS** - Special parameters ($?, $$)

## Debugging

```c
// Print all tokens with details
print_tokens(&tokens);

// Print token type summary
print_token_summary(&tokens);

// Print single token
t_token *tok = deque_idx(&tokens, 0);
print_token_debug(tok);
```
