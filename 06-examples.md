# 6. Examples

[Back to README](README.md) · Purpose: copy-paste examples you can run, with the status of each one stated honestly.

| Example | Needs SFML? | Status |
|---|---|---|
| 1. Build and run the game | Yes | Not run here (Windows + SFML) |
| 2. Using `Random.h` on its own | No | **Compiled and run here** |
| 3. Level and speed table | No | **Computed here** with the same float arithmetic |
| 4. Catching a missing font | Yes | Not compiled here |

## Example 1: Build and run the game

From the project root on Windows with MinGW, Ninja, and SFML installed as described in [02-building.md](02-building.md):

```bat
cmake -S . -B build -G Ninja
cmake --build build
cd build
RetroSnake.exe
```

Expected: a 640×480 window titled "Retro Snake" opens with a green square in the middle, a red square somewhere, gray grid lines, and the text `SCORE: 0` (bottom left), `LEVEL: 1` (bottom left, below the score), and `FPS: ...` (top right, green). The snake does not move until you press W, A, S, D or an arrow key.

## Example 2: Using `Random.h` on its own

`Random.h` only needs the standard library, so you can reuse it in any C++17 project.

```cpp
#include "Random.h"
#include <iostream>

int main()
{
    // Both arguments are int -> the plain int overload is chosen.
    int roll = Random::get(1, 6);
    std::cout << "roll in [1,6]: " << (roll >= 1 && roll <= 6 ? "ok" : "BAD") << '\n';

    // Same value computed the way Fruit::GenerateCoors does it (grid cell -> pixel).
    int cellX = Random::get(0, 640 / 20 - 1);
    int pixelX = cellX * 20;
    std::cout << "pixelX multiple of 20 and < 640: "
              << ((pixelX % 20 == 0 && pixelX < 640) ? "ok" : "BAD") << '\n';

    // Explicit result type: the three-parameter template.
    long big = Random::get<long>(10, 20);
    std::cout << "long in [10,20]: " << (big >= 10 && big <= 20 ? "ok" : "BAD") << '\n';
}
```

Build and run (Linux/macOS/MinGW shell; adjust the include path to where `Random.h` is):

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude random_example.cpp -o random_example
./random_example
```

Output observed (the random values change each run, so the program prints checks instead of the numbers):

```
roll in [1,6]: ok
pixelX multiple of 20 and < 640: ok
long in [10,20]: ok
```

It compiled with no warnings.

## Example 3: Level and speed table

How `Game::LevelUp()` changes `moveDelay`, computed with the same `float` arithmetic and the same rule (`score >= level * 20`, then subtract `0.02f` while `moveDelay > 0.05f`):

| Level reached | At score | `moveDelay` (s) after level-up | Steps per second (approx. 1 / delay) |
|---|---|---|---|
| 1 (start) | 0 | 0.15 | 6.7 |
| 2 | 20 | 0.13 | 7.7 |
| 3 | 40 | 0.11 | 9.1 |
| 4 | 60 | 0.09 | 11.1 |
| 5 | 80 | 0.07 | 14.3 |
| 6 | 100 | 0.05 | 20.0 |
| 7 | 120 | 0.03 | 33.3 |
| 8 and above | 140, 160, ... | 0.03 (no further change) | 33.3 |

Look at level 7: the code's comment says the delay should not go below 0.05, but rounding in `float` lets one more subtraction happen, so the real floor is about 0.03 s (see [known issue 1](05-usage-guidelines.md#known-behaviors-and-issues)). Because the game runs at 60 FPS, the snake can step at most once per frame, so steps per second cannot exceed 60 in practice, and at 0.03 s each step happens about every 2 frames.

## Example 4: Catching a missing font

*Illustrative and not compiled here (needs SFML 3).* The game ends with an uncaught `sf::Exception` if the font cannot be opened. A small wrapper in `Main.cpp` would turn that into a readable message:

```cpp
#include "../include/Game.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    try
    {
        Game game;
        game.Run();
    }
    catch (const sf::Exception& e)
    {
        std::cerr << "Could not start RetroSnake: " << e.what() << '\n'
                  << "Run the game from the build/ folder so ../assets can be found.\n";
        return 1;
    }
    return 0;
}
```

This is a suggestion for you to review; your source files were not modified.
