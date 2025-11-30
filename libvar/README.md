-----

# libvar: Shell Variable Management Library

`libvar` is a C library for creating, managing, and looking up shell-style variables. It is a complete implementation of the `var.c` module from a traditional `ash`/`dash`-like shell.

The primary goal of this library is to provide the full functionality of the original module while being **100% compliant with the 42 School Norminette (C89)**.

The shell has two kinds of mutable state we always need to manage: variables (name=value entries like IFS,PATH,PS1), and shell options/flags (the bits we see in $-,e.g.,-a,-x-,-v).

This lib will treat one logic the more general and let the shell itself handle its part to save/restore, export, and notify about changes in a uniform way.


-----

## Core Principles & Features

This library was rebuilt from the ground up to follow these strict rules:

  * **100% Norminette Compliant:**
      * No function exceeds 25 lines.
      * No function has more than 5 variables.
      * No function takes more than 4 parameters.
      * No forbidden loops (`for`, `do-while`, `switch`).
      * No `goto` or ternary operators.
  * **No Global Variables:** The library is 100% free of file-scope or global variables.
  * **Singleton State:** All shared state (the variable hash table, local variable stack, etc.) is managed in a single, opaque `struct` accessible only through a singleton pattern.
  * **Modular Design:** Every public function from the original `var.c` has been isolated into its own `.c` file, along with its necessary `static` helpers.

-----

## 💡 Core Concept: The Singleton

To eliminate global variables (`vartab`, `localvar_stack`, `optlist`), all persistent data is stored in a single structure: `struct s_var_state`.

You **never** access this structure directly. Instead, every function that needs to read or modify state calls the library's core accessor:

```c
struct s_var_state *state;

state = get_var_state();
```

The `get_var_state()` function returns a pointer to a `static` instance of the state. This instance is created and initialized **once** on its first call.

**Example: Accessing the hash table**

  * **Original:** `vpp = &vartab[hashval];`
  * **libvar:** `vpp = &state->vartab[hashval];`

-----

## 🚀 Public API (`libvar.h`)

This library provides the following functions to be used by your shell.

### State Management

  * `struct s_var_state *get_var_state(void)`
      * Returns a pointer to the singleton state structure.

### Core Variable Functions

  * `struct s_var *setvar(const char *name, const char *val, int flags)`
  * `intmax_t setvarint(const char *name, intmax_t val, int flags)`
  * `struct s_var *setvareq(char *s, int flags)`
  * `char *lookupvar(const char *name)`
  * `intmax_t lookupvarint(const char *name)`
  * `void unsetvar(const char *s)`

### Built-in Command Implementations

  * `int exportcmd(int argc, char **argv)`
  * `int localcmd(int argc, char **argv)`
  * `int unsetcmd(int argc, char **argv)`
  * `int showvars(const char *prefix, int on, int off)`

### Local Scope (Function) Management

  * `void mklocal(char *name, int flags)`
  * `struct s_localvar_list *pushlocalvars(int push)`
  * `void poplocalvars(void)`
  * `void unwindlocalvars(struct localvar_list *stop)`

### List & Sort Helpers

  * `char **listvars(int on, int off, char ***end)`
  * `int libvar_varcmp(const char *p, const char *q)`
  * `int var_vpcmp(const void *a, const void *b)`

-----

## External Dependencies

`libvar` is a library, not a standalone program. It **requires** the main program (your shell) to provide implementations for several utility functions. These functions are expected to be available at link time.

Your `libft` (or equivalent) **must** provide:

### 1\. Memory Management (`memalloc.h`)

  * `ckmalloc`
  * `ckfree`
  * `savestr`
  * `memcpy` (or `ft_memcpy`)

### 2\. String Utilities (`mystring.h`)

  * `atomax` (or `ft_atoi` for `intmax_t`)
  * `fmtstr` (a safe `sprintf`)
  * `endofname`
  * `strchrnul`
  * `mempcpy`
  * `strlen` (or `ft_strlen`)
  * `strchr` (or `ft_strchr`)
  * `strcmp` (or `ft_strcmp`)

### 3\. Error and Output (`error.h`, `output.h`)

  * `sh_error` (error reporting function)
  * `out1fmt` (output printing function)
  * `spcstr` (a `const char *` pointing to `" "`)
  * `nullstr` (a `const char *` pointing to `""`)

### 4\. Stack String System (`memalloc.h`)

  * `STARTSTACKSTR`
  * `stackstrend`
  * `growstackstr`
  * `grabstackstr`

### 5\. Sorting (Your `libft`)

  * `ft_qsort` (or your chosen sort algorithm, used by `showvars`)

### 6\. Shell Callbacks & Dependencies

The library assumes the main shell provides these functions for `varinit` callbacks and `unsetcmd`.

  * `changemail`
  * `changepath`
  * `getoptsreset`
  * `sethistsize`
  * `optschanged`
  * `unsetfunc`


# Depend

## Dependencias Externas

### 1. Gestión de Memoria
* `ckmalloc(size_t size)`: Un `malloc` que comprueba si falla (y llama a `sh_error`).
* `ckfree(void *ptr)`: Un `free` seguro.
* `savestr(const char *s)`: Un `strdup` que usa `ckmalloc`.

### 2. Utilidades de Cadenas
* `endofname(const char *s)`: Encuentra el fin de un nombre de variable.
* `strchrnul(const char *s, int c)`: Un `strchr` que devuelve el puntero al `\0` si no encuentra `c`.
* `fmtstr(char *buf, int size, const char *fmt, ...)`: Una versión segura de `sprintf`.
* `atomax(const char *s, int base)`: `atoi_base` pero para `intmax_t`.
* `nullstr`: Una macro o variable global de solo lectura que apunta a `""`.

### 3. Salida y Errores (`output.h`, `error.h`)
* `sh_error(const char *fmt, ...)`: La función principal para reportar errores.
* `out1fmt(const char *fmt, ...)`: La función principal para imprimir (como `printf`).
* `spcstr`: Una macro o variable global que apunta a `" "`.

### 4. Sistema de "Stack String"
* `STARTSTACKSTR(ep)`
* `stackstrend()`
* `growstackstr()`
* `grabstackstr(ep)`

### 5. Sistema de Expansión (`expand.h`)
* `single_quote(const char *s)`: Envuelve una cadena en comillas simples para `showvars`.

### 6. Control del Shell (`shell.h`)
* `INTOFF`: Macro para deshabilitar interrupciones.
* `INTON`: Macro para habilitar interrupciones.

### 7. Funciones de Callback
El `var_state.c` que creamos *asume* que estas funciones existen en el *scope* global para asignarlas como *callbacks* a las variables:
* `changemail`
* `changepath`
* `getoptsreset`
* `sethistsize`

---

## Dependencias Internas

Dentro de nuestra propia librería, las funciones ahora dependen unas de otras de esta manera (además de `get_var_state()`, que es usada por casi todas):

* **`setvar`**: Depende de `setvareq`.
* **`setvarint`**: Depende de `setvar`.
* **`setvareq`**: Depende de `var_hash` y `var_find`.
* **`lookupvar`**: Depende de `var_hash` y `var_find`.
* **`lookupvarint`**: Depende de `lookupvar`.
* **`listvars`**: No tiene dependencias internas
* **`showvars`**: Depende de `listvars` y `var_vpcmp`
* **`var_vpcmp`**: Depende de `libvar_varcmp`.
* **`exportcmd`**: Depende de `setvar`, `var_hash`, `var_find` y `showvars`.

extern void ckfree(void *ptr);
extern void *ckmalloc(size_t size);
extern char *savestr(const char *s);
extern void sh_error(const char *, ...);
extern char *strchrnul(const char *s, int c);
extern char *end_of_name(const char *s);
extern char *ft_strchrnul(const char *s, int c);
extern size_t ft_strlen(const char *s);
extern void *ckmalloc(size_t size);
extern void *ft_memcpy(void *dst, const void *src, size_t n);
// External dependencies
extern const char	*spcstr;
extern const char	*nullstr;
extern char			*strchrnul(const char *s, int c);
extern const char	*single_quote(const char *s);
extern void			out1fmt(const char *fmt, ...);
extern void			ft_qsort(void *base, int nmemb, int size,
						int (*compar)(const void *, const void *));
extern void *ft_memcpy(void *dst, const void *src, size_t n);
extern void ckfree(void *ptr);
extern void optschanged(void);
extern char *strchrnul(const char *s, int c);
// Forward declaration for pointer usage
struct s_var_state;

// External stack string helpers (provided by shell/libft)
extern char	**stackstrend(void);
extern char	**growstackstr(void);
extern char	**grabstackstr(char **);
extern void	STARTSTACKSTR(char **);