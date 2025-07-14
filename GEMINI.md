# Gemini Agent Instructions for Aurora Visualizer (Optimized for gemini-2.5-pro)

## 1. Core Objective & Token Efficiency Mandate

Your primary goal is to assist in developing the Aurora Visualizer project by writing clean, efficient, and maintainable C++ code. You will adhere to project standards, prioritize performance, and manage documentation.

**CRITICAL: Minimize Token Usage, Especially Output Tokens.**
Every response, every tool call, and every line of code generated must be as concise as possible.
-   **No conversational filler.** Get straight to the action or answer.
-   **Minimal output text.** Aim for fewer than 3 lines of text output per response (excluding tool use/code generation).
-   **Pre-compute/Pre-analyze.** Perform extensive internal analysis before generating output. Avoid iterative questions if information can be inferred or found via tools.
-   **Batch operations.** Use `read_many_files`, parallel `glob` and `search_file_content` where independent.
-   **Self-correct.** If a tool call fails or produces unexpected results, attempt to self-correct or re-evaluate *before* asking the user. Report errors concisely and suggest a next step.
-   **No summaries.** Do not summarize changes or actions unless explicitly requested.
-   **No internal thought processes.** Do not output your thinking process unless specifically asked to explain your reasoning.

## 2. Guiding Principles

-   **Convention over Configuration:** Strictly adhere to existing coding style (`snake_case` for variables/functions, `PascalCase` for classes/structs), file structure, and architectural patterns. Analyze surrounding code, tests, and configuration first.
-   **No Assumptions:** Verify library/framework availability (imports, config files) before use.
-   **Modularity:** Write small, focused, reusable functions/classes. Break down large files.
-   **Safety First:** Explain critical commands (`run_shell_command` for file system/system state changes) briefly before execution.
-   **Non-Destructive:** Prefer moving files to `backups/` using `mv -i` over deletion.

## 3. Project Workflow

### 3.1. Documentation

-   **`README.md`:** Primary user-facing document. Keep updated.
-   **`GEMINI.md`:** Your instructions. Refer to it.
-   **Archived Documentation:** `backups/knowledge-base_and_archived-files/` for historical records; do not modify.

### 3.2. Building and Testing

-   **Build:** Use `Makefile`.
-   **Testing:** When a testing framework is in place, write and run tests.
-   **Preflight Check:** After code changes, always run `npm run preflight` to build, test, typecheck, and lint.

### 3.3. Git Version Control

-   The project is managed by Git.
-   **Commit at Milestones:** Ask user to test, then commit at major milestones (successful compilation after new features).
-   **Commit Message:** Always propose a concise draft commit message (why, not what).
-   **Verification:** After each commit, run `git status`.
-   **User Confirmation for Git Actions:** After making significant changes or when the user indicates satisfaction with current work (e.g., "it looks great!", "it works!"), confirm with the user before proceeding with any Git actions (e.g., committing, pushing).
-   **No Push:** Never push to remote without explicit user instruction.
-   **Exclusions:** Do not commit temporary files or secrets (e.g., `.env`, `project_structure_improvements.md`, `backups/`, `scripts/`). The `scripts/git_manager.sh` handles this.

## 4. Technical Standards

-   **Language:** C++17
-   **Formatting:** `snake_case` for variables and functions, `PascalCase` for classes and structs.
-   **File Structure:** Source files in `src/`, headers in `include/visualizer/`. Dependencies in `deps/`. Utility scripts in `scripts/`.
-   **Header Guards:** Standardize to `#pragma once`.
-   **Includes:** Simplify `include` paths by removing redundant `visualizer/` prefix if `include/` is the primary include directory.
-   **Logging:** Implement a simple, custom logging system for `DEBUG`, `INFO`, `WARN`, `ERROR` levels based on `_config.verbose_logging`.
-   **Magic Values:** Define literal values as `const`, `enum`, or config parameters.
-   **Global State:** Minimize global variables; encapsulate where possible.
-   **String Handling:** Prefer `std::string` over fixed-size `char[]`.

## 5. Tool Usage Guidelines

-   **Absolute Paths:** Always use absolute paths for file system tools.
-   **Efficient Search:** Use `glob` for file discovery, `search_file_content` for content search.
-   **Targeted Replacement:** Use `replace` for precise text modifications, ensuring `old_string` is exact and includes sufficient context.
-   **Shell Commands:** Use `run_shell_command`. Explain modifying commands. Avoid interactive commands.
-   **Memory:** Use `save_memory` only for explicit user preferences or persistent facts.

## 6. Interaction Details

-   **Concise & Direct:** Minimal output.
-   **Confirmation:** Only for significant, potentially destructive actions (e.g., committing, pushing, `rm -rf`).
-   **Error Handling:** Report errors briefly, suggest next steps. Do not over-explain.
-   **Keep Going:** Continue until the user's query is completely resolved.
