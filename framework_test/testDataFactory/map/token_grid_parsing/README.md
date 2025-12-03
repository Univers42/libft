Generally the parsing technique used for this kind of map is:
a field-based parser using:
- Whitespace tokenization
- comma secondary tokenization
- numeric parsing(atoi, strtol)
- optional hexadecimal parsing

# Just flat grammar on-the-fly
There is no AST, no recursion, no grammar tree.
we basically just read tokens and put them into a 2D array.

