# TextManager Component Improvements

This document outlines potential improvements for the `TextManager` component.

## `src/TextManager.cpp` / `include/visualizer/TextManager.h`

*   **`sanitize_text` Logic:**
    *   **Issue:** The comment `// removed this manually because songs often have round brackets c == '(' || c == ')') {` indicates a manual change to character filtering. The current implementation uses a loop for allowed characters.
    *   **Suggestion:** If the sanitization rules are flexible or need to be easily changed, consider using `std::regex` for more complex pattern matching, or make the set of allowed characters configurable. Document the sanitization rules clearly.
*   **Complex `split_text` Function:**
    *   **Issue:** The `split_text` function is quite complex, especially the nested logic for splitting words that exceed the `max_width`.
    *   **Suggestion:** Break down `split_text` into smaller, more focused private helper functions. For example, one function to handle splitting a single word if it's too long, and another to build lines from words. This would improve readability and testability.
*   **Magic Number `0.5f` for `max_width`:**
    *   **Issue:** `const float max_width = window_width * 0.5f;` uses a magic number.
    *   **Suggestion:** Define `0.5f` as a named constant (e.g., `TEXT_WIDTH_RATIO`) in `Config` or a relevant header.
