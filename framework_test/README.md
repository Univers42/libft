# tester Message + Colors

Purpose

- message.hpp: lightweight Message class that accumulates streamable content into a string buffer and exposes GetString().
- colors.hpp: ANSI color/style escape sequences and helpers (256 / truecolor) for terminal output.
- The Message class now supports coloring its whole output by calling SetColor(...) and will automatically append a RESET when streamed to an ostream.

Quick usage

- Include headers:
  #include "log/message.hpp"
  #include "colors.hpp"

- Basic:
  tester::Message m;
  m << "Hello " << 123;
  std::cout << m << std::endl;

- Full-message coloring:
  m.SetColor(tester::colors::GREEN) << "OK";
  std::cout << m << std::endl; // prints green text and resets color afterwards

- Inline color constants also work (they are plain escape sequences):
  std::cout << tester::colors::RED << "red text" << tester::colors::RESET << std::endl;

- Helpers:
  - fg256(n), bg256(n): return std::string with 256-color escape sequences.
  - fg_rgb(r,g,b), bg_rgb(r,g,b): 24-bit color escape sequences.

Notes

- The simple wchar_t -> narrow conversion used by Message is basic and will lose non-ASCII characters. For UTF-8 preservation, convert wide strings to UTF-8 before inserting.
- Use SetColor / ClearColor to manage whole-message coloring. Directly inserting color escape sequences into the Message buffer also works.
