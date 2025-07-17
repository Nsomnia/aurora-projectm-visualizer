# Ideas

This document contains a list of ideas for future development.

## User-Added Improvements, Requests, Suggestions, or Other Thoughts

*   [ ] There must be a playlist of audio files as a form of playback queue. An option in the settings window and also as a command line option will allow this list to automatically play, and record if the record mode is enabled, first to last and ending on the last audio file song.
*   [ ] Recordings should be done to a temporary folder such as /tmp/ where zram or zswap aid in write speeds. These files should not be kept if the songs played back in record mode do not playback fully.
*   [BUG] The playback to the user cuts off at an unpredictable time before hte end of the song but the recorded video is full start to finish.

## Quick Gemini Inquiry

How do I adjust how many character count the title text has new lined placed? Can we set that font size different from the artist name as well since now its a bit large. Can it automatically adjuist the number of characters to split into a new line at the next space found (once sanitized) based on the visualizer dimensions to be more dynamically safe?

## MilkDrop Keybinds/Features

NOTE: A lot of these can be done in the qt/gtk gui only to make simpiler

**CORE:**
ESC exit help / menus
ALT+ENTER toggle fullscreen (or double-click on mouse)

**PRESETS:**
SPACE / H soft (space) / hard (H) cut to next preset
BACKSPACE go back to previous preset (with memory)
R toggle random / sequential preset order
A / SHIFT+A randomize colors / back to previous random color
Z / SHIFT+Z do random mini-mash-up / back to previous random preset
P / SHIFT+P force pattern / blending progress ( or : to reset)
CTRL+DELETE press to directly erase current preset
CTRL+INSERT save all the options + window size and position

**HOTKEYS:**
F1 help
F2 change max FPS
F3 change time between preset
F4 show preset name
F5 show preset rating
F6 always on top / no borders
F7 auto cut to next preset based on music
F8 double-preset mode
F9 toggle effects
F11 toggle sprites

**SPRITES:**
SHIFT+K 00-9 toggle sprite / message mode
DELETE clear latest sprite from the slot
CTRL+DELETE clear oldest sprite from the slot
RIGHT-CLICK move all the sprites
SCROLL-WHEEL resize sprite
G / SHIFT+J unblock saved sprite from milk file
J toggle sprite blendmodes

**PLAYER:**
CTRL+right next track
CTRL+left previous track
