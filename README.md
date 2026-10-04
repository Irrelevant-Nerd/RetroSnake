# RetroSnake Documentation

> Entry point. Start here, then follow the links.

**RetroSnake** is a small single-player Snake game written in C++17 with SFML 3.1. You steer a green snake around a 32×24 grid, eat red fruit to grow and score, and the game speeds up as you level up. Hit a wall or your own tail and the game ends.

## Contents

| File | What you will find |
|---|---|
| [01-overview.md](01-overview.md) | What the game does, controls, rules, file tree, how the classes fit together |
| [02-building.md](02-building.md) | How to build and run it, and what to do when it fails |
| [03-api-reference.md](03-api-reference.md) | Every class, function, constant, and member, with tables |
| [04-lifecycle-and-states.md](04-lifecycle-and-states.md) | Startup order, the frame loop, game states, how the snake moves and grows |
| [05-usage-guidelines.md](05-usage-guidelines.md) | Rules for changing the code safely, common mistakes, known issues |
| [06-examples.md](06-examples.md) | Runnable examples and a level/speed table (checked, see notes inside) |

There is no `07-deprecations-and-changelog.md`: the code contains no deprecated items, no TODO/FIXME comments, and no version history (the zip has no `.git` folder).

## Quick facts

| Item | Value |
|---|---|
| Language / standard | C++17 |
| Library | SFML 3.1 (Graphics, Window, System used; Audio and Network are linked but unused) |
| Build system | CMake (3.10 or newer), Ninja generator in the shipped `build/` folder |
| Compiler seen in `build/` | MinGW GCC 14.2.0 on Windows (CMake 3.30.4) |
| Window | 640×480 pixels, titled "Retro Snake", capped at 60 FPS |
| Source size | 6 `.cpp` files and 7 headers, about 500 lines |
| Executable | `RetroSnake` (one target, no separate library) |

## How these docs were checked

- All 13 source files and `CMakeLists.txt` were read in full.
- Standalone code that does not need SFML was compiled and run here with `g++ -std=c++17 -Wall -Wextra` (the `Random.h` example, the level/speed table, and the tail-growth and reverse-collision logic).
- The SFML parts (windowing, drawing, keyboard) and the Windows build were **not** compiled or run here, because SFML 3.1 and the author's MinGW setup are not available in this environment. Statements about those parts come from reading the code. The Windows build commands are derived from the files in the shipped `build/` folder.

Where the code behaves in a surprising way, it is flagged as a **Note** or **Warning**, and collected in [05-usage-guidelines.md](05-usage-guidelines.md#known-behaviors-and-issues).
