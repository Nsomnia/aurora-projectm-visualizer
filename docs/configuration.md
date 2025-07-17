# Configuration File Notes (`config/default.toml`)

This document outlines observations and potential improvements related to the `config/default.toml` file.

*   **Unresolved Comments/Questions:**
    *   **Issue:** Several comments are questions or notes to self (e.g., `NOTE: IS IT FADING? DONT WANT FADE. WANT FRAME 1 AND LAST FRAME TO BE AS IDENTICAL AS POSSIBLE`, `NOTE: WHAT? AS IN EXTRA PRE_FADE_DELAY?`).
    *   **Suggestion:** Resolve these questions and update the comments to be clear and informative for end-users.
*   **`broken_preset_directory` Logic:**
    *   **Issue:** The comment `I THINK THIS LOGIC IS BROKEN, AND WE HAVE THE ENABLE/DISABLE (FAVORITES) "F" KEY ANYWAY SO THIS LOGIC I THINK COULD BE REMOVED YES/NO?` indicates a design decision.
    *   **Suggestion:** Clarify the purpose of `broken_preset_directory`. If the "mark broken" feature is indeed redundant or broken, remove it from both the config and the `PresetManager` code.
*   **`presets_directory` Path Clarification:**
    *   **Issue:** The comment `THE /USR/SHARE/PROJECTM/PRESETS IS STILL A DEFAULT AND THIS IS ADDITONAL DIRECTORY SECONDARY RIGHT? ABSOLUTE PATH?` needs clarification.
    *   **Suggestion:** Clearly state if `presets_directory` expects an absolute or relative path, and if it's the primary or an additional directory for presets.
