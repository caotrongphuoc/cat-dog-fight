<h1 align="center">Coding rules and style guide</h1>

This document defines the naming conventions, code style, commit message format, and document naming conventions used in Cat vs Dog Fight, along with instructions for installing and using `clang-format`. The goal is to keep code from every contributor consistent in form and easy to track through search tools, code review, and version control. Examples come from the existing `cd_game_*` objects and `scr_game_*` screens.

---

## Table of contents

- [I. Naming conventions](#i-naming-conventions)
  - [1. Folder](#1-folder)
  - [2. Source and header files](#2-source-and-header-files)
  - [3. Header guard](#3-header-guard)
  - [4. Macros and compile-time constants](#4-macros-and-compile-time-constants)
  - [5. Signal (enum values)](#5-signal-enum-values)
  - [6. Task ID](#6-task-id)
  - [7. Data types and typedef](#7-data-types-and-typedef)
  - [8. Functions](#8-functions)
  - [9. Variables](#9-variables)
- [II. Code style (clang-format)](#ii-code-style-clang-format)
- [III. Installing clang-format](#iii-installing-clang-format)
- [IV. Running clang-format](#iv-running-clang-format)
- [V. Commit message convention](#v-commit-message-convention)
- [VI. Document file naming convention](#vi-document-file-naming-convention)
- [VII. References](#vii-references)

---

## I. Naming conventions

The conventions below are drawn directly from the existing source code. Follow them when you extend the game so that tooling, search, and reviewers all work consistently.

**Case styles used in this document:**

| Style | Description | Example in project | Applied to |
|---|---|---|---|
| `lower_snake_case` | Lowercase letters, words separated by underscore `_` | `hit_ticks`, `cd_game_missile_handle` | Variables, functions, typedefs, source file names, folder names |
| `UPPER_SNAKE_CASE` | Uppercase letters, words separated by underscore `_` | `POWER_VALUE_MAX`, `CD_GAME_MISSILE_RUN`, `CD_GAME_MISSILE_ID` | `#define` constants, enum values, signals, task IDs, macros |
| `kebab-case` | Lowercase letters, words separated by hyphen `-` | `01-guide-coding-rules.md` | Documentation file names under `docs/` |

### 1. Folder

Use `lower_snake_case` for folder names. Organize by feature (feature-based), not by file type.

```
application/sources/app/
  game/game_cat_dog/      # source code of every game object (match, cat, dog, missile, link, cpu, ...)
  screens/                # source code of every display screen
  app_eeprom.cpp/.h       # the game record (mute, level, wins) in EEPROM
  shell.cpp               # console commands, including btn, stt, lnk and rf
application/sources/driver/
  Adafruit_oled_drv/      # OLED driver, sends only the rows that changed
application/sources/platform/stm32l/
  io_cfg.c/.h             # the only place for GPIO, pin and register code
```

The AK kit folders (`ak/` except `ak.cfg.mk` and the `CD_GAME_DEFINE_SIG` line in `ak.h`, `boot/`, `sys/`, `common/`, `networks/`, `libraries/` and the rest of `platform/`) are not modified.

### 2. Source and header files

Source and header files always carry a module prefix so you can identify the module right from the file name:

| Prefix | Meaning | Example |
|---|---|---|
| `cd_game_*` | Object belonging to the game | `cd_game_missile.cpp`, `cd_game_link.h` |
| `scr_game_*` | Handler of a game screen | `scr_game_cat_dog.cpp`, `scr_game_over.h` |

File extensions: `.h` for headers, `.cpp` for implementation (the project is built as C++).

### 3. Header guard

Use the pattern `__<FILE_NAME>_H__`, fully uppercased, matching the file name exactly:

```cpp
#ifndef __CD_GAME_MISSILE_H__
#define __CD_GAME_MISSILE_H__
...
#endif //__CD_GAME_MISSILE_H__
```

### 4. Macros and compile-time constants

Use `UPPER_SNAKE_CASE` for names. Always wrap numeric values in parentheses to avoid errors when the macro gets expanded, and put a `//` comment line above each group.

**Mandatory rule: a macro that belongs to an object or a screen MUST carry that object's or screen's name as a prefix.**

Pattern: `<OBJECT>_<PROPERTY>`. The object always comes first. Reading the macro name tells you immediately which module it belongs to, and grepping by object name returns every constant of that module.

| Constant type | Correct form |
|---|---|
| Limit | `POWER_VALUE_MAX`, `WIND_SPEED_MAX`, `CAT_HP_MAX` |
| Time | `CAT_HIT_TICKS`, `BORDER_HIT_TICKS`, `LINK_LOST_TIMEOUT` |
| Physics | `MISSILE_GRAVITY`, `MISSILE_SPEED_MIN`, `MISSILE_WIND_FORCE` |
| Position | `CAT_AXIS_X`, `BORDER_AXIS_Y`, `CAT_HAND_OFFSET_Y` |
| Bitmap size | `CAT_SIZE_BITMAP_X`, `MISSILE_BONE_SIZE_BITMAP_Y` |
| Screen layout | `MENU_ITEM_AXIS_Y`, `GAME_OVER_HINT_AXIS_X` |

Examples done right:

```cpp
// cd_game_power.h
// Power range, it sweeps up and down while MODE is held
#define POWER_VALUE_MAX (100)

// Power added or removed every tick while charging
#define POWER_STEP (4)

// cd_game_missile.h
// Gravity added to the vertical speed every tick (0.25 px/tick^2)
#define MISSILE_GRAVITY (64)

// cd_game_link.h
// No message from the rival for this long while playing means the link is lost
#define LINK_LOST_TIMEOUT (6000)
```

Enum values of an object follow the same rule (`MATCH_STATE_PLAY`, `MATCH_TURN_CAT`, `MISSILE_RESULT_HIT_BORDER`, `POWER_STATE_LOCKED`).

**Exception (project level macros):** macros that belong to the whole game (not tied to any specific object) use the project prefix `CD_GAME_*`. They live in `cd_game_match.h`, the HUD layout lives in `scr_game_cat_dog.h`:

```cpp
// Ground line, every object stands on it
#define CD_GAME_GROUND_AXIS_Y (63)

// HP lost by a player hit by a fish or a bone
#define CD_GAME_HIT_DAMAGE (20)

// HP bars in the top corners, cat on the left, dog on the right
#define CD_GAME_HUD_HP_BAR_WIDTH (44)
```

Group related constants in the right module header. Never leave magic numbers scattered across `.cpp` files; a constant table used by one `.cpp` only (the cosine table in `cd_game_missile.cpp`) may stay at the top of that file, still with its prefix and comment.

### 5. Signal (enum values)

Signals are the **public contract** between tasks. Always use the full prefix; no abbreviations, not in comments, documentation, or sequence diagrams.

| Pattern | Applied to | Example |
|---|---|---|
| `CD_GAME_<OBJECT>_<ACTION>` | Game object signal | `CD_GAME_POWER_CHARGE`, `CD_GAME_MATCH_TURN_END` |
| `CD_GAME_LINK_RX_<MESSAGE>` | Message received from the other kit | `CD_GAME_LINK_RX_THROW`, `CD_GAME_LINK_RX_ACK` |
| `CD_GAME_<ACTION>` | Game screen signal | `CD_GAME_TIME_TICK`, `CD_GAME_EXIT_GAME` |
| `AC_DISPLAY_<ACTION>` | Display task signal | `AC_DISPLAY_BUTTON_MODE_RELEASED`, `AC_DISPLAY_RENDER_SCREEN` |

Declare each task's signal set in `app.h` as its own block, anchored to `CD_GAME_DEFINE_SIG` (defined in `ak/inc/ak.h`, next to `AK_USER_DEFINE_SIG`). Timer intervals of the block go under `/* define timer */`, in milliseconds. Do not run `clang-format` on `app.h`:

```cpp
/*****************************************************************************/
/*  Cat dog fight game 'CPU' task define
 */
/*****************************************************************************/
/* define timer */
#define CD_GAME_CPU_THINK_INTERVAL									(800)
/* define signal */
enum {
	CD_GAME_CPU_PLAY = CD_GAME_DEFINE_SIG,
	CD_GAME_CPU_THINK,
	CD_GAME_CPU_RESET,
};
```

Signals that carry data are posted as common messages. The handler checks `get_data_len_common_msg()` before reading the payload (`CD_GAME_CAT_AIM_SET`, `CD_GAME_WIND_SET`, `CD_GAME_MATCH_RESULT`).

### 6. Task ID

Use the pattern `CD_GAME_<OBJECT>_ID`, fully uppercased, registered in `task_list.h`:

```cpp
CD_GAME_MATCH_ID
CD_GAME_MISSILE_ID
CD_GAME_LINK_ID
```

The corresponding handler in `task_list.cpp` keeps the same name, replacing the `_ID` suffix with `_handle` and lowercased:

```cpp
{CD_GAME_MISSILE_ID, TASK_PRI_LEVEL_4, cd_game_missile_handle},
```

Keep the enum order in `task_list.h` identical to `app_task_table[]`. Both kits run the same task table, so a task ID sent over RF24 (`if_des_task_id`) names the same task on the other kit.

### 7. Data types and typedef

Use `lower_snake_case` with the `cd_game_<object>_` prefix and the `_t` suffix. The struct stays anonymous; the typedef is the public name:

```cpp
typedef struct
{
	uint8_t state;
	uint8_t value;
	bool rising;
} cd_game_power_t;
```

The object's main state type is `cd_game_<object>_t` (`cd_game_match_t`, `cd_game_missile_t`). A message payload is named after the message it carries and is `packed` (`cd_game_link_throw_t`, `cd_game_link_result_t`). Types provided by the framework follow the same pattern (`ak_msg_t`, `view_screen_t`).

### 8. Functions

Use `lower_snake_case` with the module name as prefix, so that grepping the prefix returns every entry point of that module:

```cpp
extern int16_t cd_game_missile_cos(uint8_t angle);
extern uint8_t cd_game_missile_predict(uint8_t thrower, uint8_t angle, uint8_t power_value, int8_t wind_speed);
extern bool cd_game_record_read(cd_game_record_t* data);
```

Public functions are declared with `extern` in the module's header. Task handlers (`cd_game_missile_handle`) are declared only in `task_list.h`. Helpers used inside one `.cpp` only are `static` and keep the same prefix (`cd_game_missile_step`, `cd_game_match_next_turn`, `cd_game_cpu_plan`).

Every object header has the same layout, and its `.cpp` includes only that header:

```cpp
#ifndef __CD_GAME_POWER_H__
#define __CD_GAME_POWER_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

// drivers (buzzer.h, view_render.h, ...), then related objects

// Power range, it sweeps up and down while MODE is held
#define POWER_VALUE_MAX (100)

typedef struct
{
	uint8_t state;
	uint8_t value;
	bool rising;
} cd_game_power_t;

extern cd_game_power_t power;

#endif //__CD_GAME_POWER_H__
```

A screen header adds `sys_ctrl.h`, `sys_dbg.h`, `view_render.h`, `screens.h` and `screens_bitmap.h` to the include block and declares its view, screen and handler first. The same three lines are listed in `screens.h`:

```cpp
extern view_dynamic_t dyn_view_game_cat_dog;
extern view_screen_t scr_game_cat_dog;
extern void scr_game_cat_dog_handle(ak_msg_t* msg);
```

### 9. Variables

Use `lower_snake_case`. Do not start names with an underscore.

- **Object state:** one instance per object, named after the object, type `cd_game_<object>_t`. Declare it `extern` in the object's header and define it exactly once in the object's `.cpp`. Only that object's handler writes it; other tasks post a signal, and screens and the shell only read it.

  ```cpp
  // cd_game_missile.h
  extern cd_game_missile_t missile;

  // cd_game_missile.cpp
  cd_game_missile_t missile;
  ```

- **Module-internal variables:** declare `static` in the `.cpp` (`menu_item` in `scr_game_menu.cpp`).

- **Constant tables:** `static const` so they stay in flash (`missile_cos_table`, `menu_item_name`).

- **Local variables:** short, describe the role accurately. Keep large locals out of handlers: about 2.7 KB of stack is left.

- **No `snprintf`/`sprintf`:** they pull the full newlib printf in (about 24 KB of flash). Print numbers with `view_render.print()`.

---

## II. Code style (clang-format)

The repo already includes a `.clang-format` file at the root, shown here for reference:

```yaml
Language: Cpp
BasedOnStyle: LLVM
UseTab: ForIndentation
IndentWidth: 4
TabWidth: 4
ColumnLimit: 0
BreakBeforeBraces: Allman
AllowShortIfStatementsOnASingleLine: false
AllowShortFunctionsOnASingleLine: None
AllowShortBlocksOnASingleLine: false
AllowShortCaseLabelsOnASingleLine: false
PointerAlignment: Left
SpaceBeforeParens: ControlStatements
IndentCaseLabels: false
SortIncludes: false
```

What the non-default settings do:

| Setting | Effect |
|---|---|
| `UseTab: ForIndentation`, `IndentWidth: 4`, `TabWidth: 4` | Tabs are used only for indentation, not alignment. One tab equals 4 columns. |
| `ColumnLimit: 0` | No automatic line wrapping. The author decides where to break a line. |
| `BreakBeforeBraces: Allman` | The `{` always sits on its own line, applies to `if`, `for`, and function bodies. |
| `AllowShort*OnASingleLine: false` | One statement per line, even for short `if` statements or empty functions. |
| `PointerAlignment: Left` | Write `int* p`, not `int *p`. |
| `SpaceBeforeParens: ControlStatements` | `if (x)`, `for (...)` have a space; `func(x)` does not. |
| `IndentCaseLabels: false` | `case` labels sit at the same indent level as `switch`. |
| `SortIncludes: false` | Include order is meaningful (BSP -> framework -> project) and must not be sorted automatically. |

Only the game files (`game/game_cat_dog/*`, `screens/scr_game_*`) are formatted. `app.h`, `task_list.cpp`, `shell.cpp`, `app_eeprom.*`, the OLED driver and the other AK kit files keep the kit's own layout (column-aligned tables and `enum {` blocks). Do not run `clang-format` on them.

---

## III. Installing clang-format

Instructions for Linux (Ubuntu / Debian).

Install:

```bash
sudo apt update
sudo apt install clang-format
```

Verify the install:

```bash
clang-format --version
```

Expected output (version number may differ):

```
Ubuntu clang-format version 14.x.x
```

---

## IV. Running clang-format

### 1. Running from the command line

Format a single file in place:

```bash
clang-format -i application/sources/app/game/game_cat_dog/cd_game_missile.cpp
```

Check a file without changing it (prints a warning for every line that would change):

```bash
clang-format --dry-run application/sources/app/game/game_cat_dog/cd_game_missile.cpp
```

Format every game object and game screen:

```bash
clang-format -i application/sources/app/game/game_cat_dog/*.cpp \
    application/sources/app/game/game_cat_dog/*.h \
    application/sources/app/screens/scr_game_*
```

Format only the files you change. Reformatting untouched files in a feature commit hides the real change.

### 2. VSCode integration

**Step 1.** Install the **C/C++** extension (Microsoft). It ships with `clang-format` and automatically picks up the `.clang-format` file in the repo.

**Step 2.** Open the workspace settings (`.vscode/settings.json`) and add the following config:

```json
{
    "editor.formatOnSave": true,
    "editor.defaultFormatter": "ms-vscode.cpptools",
    "C_Cpp.clang_format_style": "file",
    "C_Cpp.clang_format_fallbackStyle": "LLVM"
}
```

**Step 3.** Format the current file using the shortcut <kbd>Ctrl</kbd>+<kbd>Shift</kbd>+<kbd>I</kbd> (Windows / Linux).

**Step 4.** Turn on `Format On Save` so the editor formats the file on every save, making sure no code with stale formatting gets committed. Turn it off, or use a folder-level setting, before editing the kit files listed in [II](#ii-code-style-clang-format).

---

## V. Commit message convention

Every commit must follow the format `[ACTION] short description` so the git history stays easy to read and easy to filter.

### 1. Workflow

```bash
git add <path>                                # stage the files of one topic
git commit -m "[ACTION] short description"    # tag is mandatory, keep description short
git push                                      # push to remote
```

Stage specific paths instead of `git add .` to avoid committing build output or junk files by mistake.

### 2. Action tags

| Tag | When to use |
|---|---|
| `[ADD]` | Adding a new file, feature, asset, or document |
| `[UPDATE]` | Updating existing code (refactor, rename, tweak logic, bump version) |
| `[FIX]` | Fixing an existing bug, build error, or formatting error |
| `[REMOVE]` | Removing a file, feature, or dead code |
| `[DOC]` | Documentation-only changes (`docs/`, `README.md`, large comment blocks) |
| `[MERGE]` | Branch merges (usually tool-generated, do not hand-edit) |

### 3. Description style

- Tag fully uppercased inside `[]`, followed by exactly one space, then the description.
- Description in lowercase, imperative mood (`add`, `fix`, `rename`, `move`...), no trailing period.
- Keep the length around 70 characters. If longer, shorten it or move the details into the commit body.
- When the change touches a specific module / signal / file, name it directly so the history is easy to grep.

### 4. Good examples

```text
[ADD] add power bar, aiming and missile flight with wind
[ADD] add rf24 match link and redraw only changed oled rows
[ADD] add link heartbeat, result ack and rematch
[UPDATE] switch oled driver to sh1106 for kit ver2
[REMOVE] remove idle ball screen to free flash
[DOC] add readme, coding rules and sequence docs
```

### 5. Examples to avoid

```text
update                          # missing tag
[update] fix something          # tag must be uppercase
[ADD] Added new file.           # no past tense and no trailing period
[FIX] fix bug                   # too vague, which bug?
[ADD] cd_game_cat.cpp + cd_game_dog.cpp + scr_game_menu.cpp ... # too long, group by topic
```

---

## VI. Document file naming convention

Files in `docs/` follow the format `<NN>-<category>-<topic>.md`:

| Component | Convention | Example |
|---|---|---|
| `NN` | A 2-digit sequence number, starting from `01`. Reflects reading order: guides come first, design docs come after. | `01`, `02`, `03` |
| `category` | Document category. Only use predefined values; do not add new categories. | `guide`, `design` |
| `topic` | Main topic, written in `kebab-case` (lowercase, words separated by `-`). | `coding-rules`, `sequence-object` |

Categories currently in use:

| Category | Purpose | Typical content |
|---|---|---|
| `guide` | Workflow and process guides for contributors | Coding rules |
| `design` | Describes architecture and runtime behavior of the system | Sequence diagrams |

Files currently in the repo:

```
docs/
├── 01-guide-coding-rules.md
├── 02-design-sequence-object.md
└── 03-design-sequence-runtime.md
```

A few important notes:

- Documentation files (`.md`) use `kebab-case` (hyphens). Source files and folders use `snake_case` (underscores). The difference is intentional: `kebab-case` is the standard convention for Markdown slugs and URLs.
- Images that go with a document live under `resources/images/<topic>/`, named after what they show. For example: `resources/images/objects/table/cd_game_missile.gif`.
- Sequence diagrams are written in Mermaid inside the document. Do not use `;` inside a Mermaid label, it breaks the diagram.
- When adding a new document, continue the sequence number from the current maximum and keep the same category together to preserve a natural reading order.
- Renaming a documentation file must be done with `git mv` so that the rename history is tracked properly.

---

## VII. References

- [Naming convention: Multi-word identifiers (Wikipedia)](https://en.wikipedia.org/wiki/Naming_convention_(programming)#Multiple-word_identifiers): definitions of `snake_case`, `SCREAMING_SNAKE_CASE`, and `kebab-case` used in Sections I and VI.
- [Clang-Format: Configurable Format Style Options](https://clang.llvm.org/docs/ClangFormatStyleOptions.html#configurable-format-style-options): describes every key in the `.clang-format` file from Section II.
- [LLVM Coding Standards: Source Code Formatting](https://llvm.org/docs/CodingStandards.html#source-code-formatting): base style inherited via `BasedOnStyle: LLVM` in Section II.
- [Pro Git: Recording Changes to the Repository](https://git-scm.com/book/en/v2/Git-Basics-Recording-Changes-to-the-Repository): the `git add` / `git commit` / `git push` workflow used in Section V.
- [Mermaid: Sequence diagrams](https://mermaid.js.org/syntax/sequenceDiagram.html): syntax of the diagrams in the design documents.

---

## Contact & Support

<p style="font-size: 20px;"><strong>Cao Trong Phuoc</strong> - Software Engineer - Embedded Systems</p>

```text
Thank you for visiting this repository.
If you have any questions, suggestions, or feedback about this project or firmware development, feel free to contact me directly.
```

<a href="https://github.com/caotrongphuoc">
  <img src="https://img.shields.io/badge/GitHub-caotrongphuoc-181717?style=for-the-badge&logo=github&logoColor=white" alt="GitHub"/>
</a>

<a href="https://www.linkedin.com/in/cao-trong-phuoc/">
  <img src="https://img.shields.io/badge/LinkedIn-Cao%20Trong%20Phuoc-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white" alt="LinkedIn"/>
</a>
