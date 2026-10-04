# 5. Usage Guidelines

[Back to README](README.md) · Purpose: how to change the code safely, mistakes to avoid, and a list of behaviors you may not expect.

## Ownership and lifetime

| Question | Answer |
|---|---|
| Who creates objects? | `main()` creates one `Game`; `Game` creates everything else as members |
| Who frees them? | Destructors, automatically. There is no `new`, `delete`, `malloc`, or `free` in the project |
| Do getters return pointers? | No. `getSnakePosition`, `getFruitPosition` return `sf::Vector2f` copies; `getTailPosition` returns a **copy** of the list. Nothing returned can dangle |
| Does `Text`'s font stay alive? | Yes: `font` and the `sf::Text` objects are members of the same `Text` object, and `font` is declared first |

## Thread safety

Single-threaded. `Random` uses one global engine (`Random::mt`) with no locking; do not call it from several threads.

## Preconditions

| Where | Precondition |
|---|---|
| Running the exe | Working directory is one level below the project root, so `../assets/fonts/HomeVideo-BLG6G.ttf` exists |
| `Random::get(min, max)` | `min <= max`; integer types only |
| `Snake::Move(moveDelay)` | `moveDelay >= 0` seconds |
| Constants | `wWIDTH` and `wHEIGHT` are multiples of `GRID_SIZE` (see below) |

## Changing the window or grid size

Most of the code uses `wWIDTH`, `wHEIGHT`, and `GRID_SIZE`, but not all of it. If you change them, also review:

| Location | Hard-coded value | Problem |
|---|---|---|
| `Snake::Snake()` | `(wWIDTH/2) / 20 * 20` and `(wHEIGHT/2) / 20 * 20` | Uses the literal `20` instead of `GRID_SIZE`, so the start cell is only aligned if `GRID_SIZE` is 20 |
| `Text::Text()` | `wWIDTH-635`, `wHEIGHT-55`, `wHEIGHT-25`, `wWIDTH-170`, `wHEIGHT-470` | These offsets only look right for 640×480 (they reduce to x=5, y=425, y=455, x=470, y=10). A different window size will misplace the text |
| `Snake` `SPEED` | `20.f` | Must equal `GRID_SIZE`, or the snake will not stay on the grid and `Eat` (exact equality) will stop working |

Keep `wWIDTH` and `wHEIGHT` multiples of `GRID_SIZE`, so the border check and the fruit positions line up with whole cells.

## Common mistakes

1. **Running from the wrong folder.** Starting `RetroSnake.exe` from the project root, or from an IDE whose working directory is somewhere else, makes the font load fail and the program ends with an uncaught `sf::Exception`. Run from `build/`, or change the font path to an absolute or exe-relative one.
2. **Forgetting the DLLs.** A fresh build folder has no SFML DLLs; copy them next to the exe.
3. **Changing the speed constant only.** `Snake::SPEED` is the step size in pixels, not a time. To make the game faster or slower, change `Game::moveDelay` (seconds between steps).
4. **Comparing floats loosely or moving by non-cell amounts.** `Snake::Eat` uses exact `==` on floats. This is fine only while every position is a multiple of 20.
5. **Using `Fruit::GenerateCoors()` directly after the game has started.** It does not avoid the snake. Use `Game::ValidateFruitCoors()`, which re-rolls until the spot is free.
6. **Calling `getTailPosition()` in a hot loop.** It copies the list on every call. Store the result in a local variable first if you need to loop over it more than once.
7. **Copy-pasting include guards.** Guard names here are short, generic words (`GAME`, `GRID`, `TEXT`, `FRUIT`, `SNAKE`, `CONSTANTS`). `TEXT` is also the name of a macro in the Windows API headers, so if you ever include `<windows.h>` next to `Text.h`, expect a clash. Prefer names like `RETROSNAKE_TEXT_H` or `#pragma once`.
8. **Forgetting that `Render()` does not render.** Drawing happens in `Draw()`. Put update logic in `Render()` and drawing code in `Draw()`.

## Known behaviors and issues

Found by reading the code. Items marked *verified* were reproduced with standalone C++ that copies the same logic. Nothing was changed in your source.

| # | Behavior | Severity | Details |
|---|---|---|---|
| 1 | **Speed floor is lower than the comment says** (*verified*) | Low | `LevelUp` is meant to stop speeding up at 0.05 s ("we dont want the snake to be way too fast"). With float math, `0.15 - 5 × 0.02` is slightly above `0.05f`, so the check `moveDelay > 0.05f` passes once more. The delay ends at about **0.03 s from level 7 onward**. See the table in [the examples](06-examples.md#example-3-level-and-speed-table) |
| 2 | **Instant reverse kills the snake once it has 2+ segments** (*verified*) | Medium (gameplay) | `Direction()` does not block turning 180°. With 0 or 1 tail segments a reverse is harmless; with 2 or more, the head lands on the tail and the game ends |
| 3 | **Game-over frame is never shown** | Low | The collision check, 2-second sleep, and `close()` run before `Draw()`, so the screen freezes on the frame *before* the fatal move, and `Draw`/`display` then run on a closed window. See [the lifecycle doc](04-lifecycle-and-states.md#game-over-what-actually-happens) |
| 4 | **Infinite loop if the board fills** | Low (very hard to reach) | When the snake occupies all 768 cells, `ValidateFruitCoors()` can never find a free cell and the game hangs. No win condition exists |
| 5 | **First fruit can spawn under the snake** | Very low | The constructor does not call `ValidateFruitCoors`. If the fruit lands on the start cell (320, 240) — 1 chance in 768 — it is "eaten" on the first frame |
| 6 | **Font failure ends the program** | Medium (setup) | Relative font path plus no `try/catch` around `Game game;` in `main` |
| 7 | **`getTailPosition()` copies the list each call** | Low | Called every frame by the self-collision check, and again on every re-roll in `ValidateFruitCoors()`. Returning a `const` reference would avoid the copies |
| 8 | **Unused things** | Cosmetic | `Snake::getSnakeSize`; `<list>` included in `Fruit.h`; `<iostream>` included in `Main.cpp`; `HomeVideoBold-R90Dv.ttf`; SFML Audio and Network (linked, never used); `Fruit::fruitCoorsX/Y` duplicate the shape's position |
| 9 | **Committed build output** | Cosmetic | The zip includes `build/` (objects, `.exe`, DLLs, CMake cache with absolute `C:/` paths). These are machine-specific and normally left out of version control |
| 10 | **Getters are not `const`** | Cosmetic | `getSnakePosition()`, `getFruitPosition()`, etc. are non-const member functions, so they cannot be called on a `const` object |

## Suggested documentation comments (optional)

Not applied to your files. Example of how a header could carry the contract using Doxygen:

```cpp
/**
 * @brief Moves the snake one cell if enough time has passed.
 *
 * @param moveDelay Seconds that must pass between steps (>= 0).
 * @note Does nothing while the snake has no direction (before the first key press).
 * @note Grows by one segment on the step after Eat() returned true.
 */
void Move(float moveDelay);
```

If you want, Claude can generate a full set of these blocks for every header, plus a `Doxyfile`.
