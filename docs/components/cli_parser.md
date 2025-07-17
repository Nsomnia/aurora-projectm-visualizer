# CliParser Component Improvements

This document outlines potential improvements for the `CliParser` component.

## `src/CliParser.cpp` / `include/visualizer/CliParser.h`

*   **Verbose `parse` Function:**
    *   **Issue:** The `parse` function is quite long due to the extensive `unordered_map` setup for command-line arguments.
    *   **Suggestion:** Consider a more data-driven approach for defining CLI options. This could involve a `std::vector` of structs, where each struct defines an option (name, type, description, and a lambda for parsing). This would make the `parse` function more generic and easier to extend.
*   **Long `display_help` Function:**
    *   **Issue:** The `display_help` function is very long and prints a large block of text.
    *   **Suggestion:** Break down `display_help` into smaller, more manageable private static functions, each responsible for printing a specific section of the help message (e.g., `display_display_options_help()`, `display_text_options_help()`).
