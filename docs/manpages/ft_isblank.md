# ft_isblank

## im med

---


## Synopsis

```c
int ft_isblank(int c);
```

## Description

Returns non-zero if `c` is a blank character in the "C" locale. Recognized blanks are:

- Space `' '`
- Horizontal tab `'\t'`

## Return Value

1 if blank, 0 otherwise.

## Notes

Use with values representable as `unsigned char` or `EOF`.
