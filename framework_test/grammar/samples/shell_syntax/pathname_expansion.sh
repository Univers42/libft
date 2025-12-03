#!/bin/sh
# pathname (globbing) expansion examples

# basic globbing
echo "Matches *.sh in current dir:"
echo *.sh

# character classes and ranges
echo "Matches files like test[0-9].txt:"
echo test[0-9].txt

# question mark matches single char
echo "Single-char match: ?est matches:"
echo ?est

# escaping to prevent globbing
echo "Literal a* (escaped):" a\*

# brace expansion (bash-specific; often not supported in pure sh)
# echo file{1,2}.txt

# note: behavior when no match: many shells leave the pattern unchanged, some support nullglob/failglob
# this sample is for tokenizer/parser recognition rather than execution correctness in all shells
