# 3. API Reference

[Back to README](README.md) · Purpose: look up any class, function, constant, or member.

Everything here is internal to this one executable: there is no installed library. "Public" and "private" below mean the C++ access level.

**Conventions.** Positions are pixels in window coordinates, `(0,0)` at the top-left, `x` to the right, `y` down. A position always means a shape's top-left corner. `sf::Vector2f` is SFML's float pair (`.x`, `.y`). "Internal" marks private members, listed so you can understand the code.

| Module | Header | Source |
|---|---|---|
| [Constants](#constants) | `include/Constants.h` | (none) |
| [Random](#random) | `include/Random.h` | (none, header-only) |
| [Grid](#grid) | `include/Grid.h` | `src/Grid.cpp` |
| [Fruit](#fruit) | `include/Fruit.h` | `src/Fruit.cpp` |
| [Snake](#snake) | `include/Snake.h` | `src/Snake.cpp` |
| [Text](#text) | `include/Text.h` | `src/Text.cpp` |
| [Game](#game) | `include/Game.h` | `src/Game.cpp` |
| [main](#main) | (none) | `src/Main.cpp` |

---

## Constants

`include/Constants.h` (include guard `CONSTANTS`). Global `constexpr int` values.

| Name | Value | Meaning |
|---|---|---|
| `wWIDTH` | `640` | Window width in pixels |
| `wHEIGHT` | `480` | Window height in pixels |
| `GRID_SIZE` | `20` | Side of one grid cell in pixels |

Derived: 640 / 20 = **32 columns**, 480 / 20 = **24 rows**, **768 cells**.

> **Note.** The code is only consistent for these values. See [changing the constants](05-usage-guidelines.md#changing-the-window-or-grid-size).

---

## Random

`include/Random.h` (include guard `RANDOM_MT_H`). Header-only, uses only the standard library. Namespace `Random`.

| Name | Signature | Description |
|---|---|---|
| `generate` | `inline std::mt19937 generate()` | Builds a Mersenne Twister engine seeded from the clock plus seven `std::random_device` values. Called once, to create `mt` |
| `mt` | `inline std::mt19937 mt` | The single shared engine used by every `get` call (C++17 `inline` variable) |
| `get` (int) | `inline int get(int min, int max)` | Uniform random integer in **[min, max], both ends included** |
| `get` (same type) | `template<typename T> T get(T min, T max)` | Same, for another integer type `T`. Uses `std::uniform_int_distribution<T>`, so `T` must be an integer type |
| `get` (explicit result) | `template<typename R, typename S, typename T> R get(S min, T max)` | Casts both bounds to `R`, then calls `get<R>`. Call it as `Random::get<long>(a, b)` |

Preconditions: `min <= max`. Not thread-safe (one shared engine).

---

## Grid

Draws the light-gray grid lines over the playfield. Has no state that changes after construction.

### Constructor

| Function | Description |
|---|---|
| `Grid()` | Sizes the two reusable line shapes: `gridRow` is `wWIDTH × 1.5` px, `gridColumn` is `1.5 × wHEIGHT` px. Both are gray `(128,128,128)` |

### Methods

| Function | Parameters | Returns | Description |
|---|---|---|---|
| `void Draw(sf::RenderWindow& window)` | `window`: target to draw on | nothing | Draws horizontal lines at `y = 20, 40, ... 480` (24 lines) and vertical lines at `x = 20, 40, ... 640` (32 lines) by moving the same two shapes and drawing each |

> **Note.** The last horizontal line (`y = 480`) and last vertical line (`x = 640`) sit exactly on the window edge, so they fall outside the visible area. You see 23 horizontal and 31 vertical lines.

### Members (internal)

| Member | Type | Purpose |
|---|---|---|
| `gridRow` | `sf::RectangleShape` | Reused for every horizontal line |
| `gridColumn` | `sf::RectangleShape` | Reused for every vertical line |
| `LINE_THICKNESS` | `const float` = `1.5f` | Line thickness in pixels |

---

## Fruit

The red square the snake eats.

### Constructor

| Function | Description |
|---|---|
| `Fruit()` | Sets size to one cell, color red, then calls `GenerateCoors()`. The first position is **not** checked against the snake |

### Methods

| Function | Parameters | Returns | Description |
|---|---|---|---|
| `void GenerateCoors()` | none | nothing | Picks a random column in `[0, 31]` and row in `[0, 23]` with `Random::get`, multiplies by `GRID_SIZE`, and moves the shape there. Does not check for the snake |
| `void Draw(sf::RenderWindow& window)` | `window` | nothing | Draws the fruit |
| `sf::Vector2f getFruitPosition()` | none | top-left position of the fruit | Always a multiple of 20 in both axes |

### Members (internal)

| Member | Type | Purpose |
|---|---|---|
| `fruit` | `sf::RectangleShape` | The drawn square |
| `fruitCoorsX`, `fruitCoorsY` | `int`, default `0` | Last generated pixel position; only used to set the shape's position (the shape also stores it) |

---

## Snake

The player's snake: a head, a list of body segments, and movement state.

### Constructor

| Function | Description |
|---|---|
| `Snake()` | Head is one green cell at pixel `(320, 240)`. The reusable `segment` shape is one green cell. The snake is **stationary** (both speeds `0`) and the tail is empty |

### Methods

| Function | Parameters | Returns | Description |
|---|---|---|---|
| `void Direction()` | none | nothing | Reads the keyboard *right now* with `sf::Keyboard::isKeyPressed`. Checks Up/W, then Down/S, then Left/A, then Right/D; the first held key sets the velocity to `±SPEED` on one axis and `0` on the other. If no key is held, the previous direction is kept. Does **not** block reversing |
| `void Move(float moveDelay)` | `moveDelay`: seconds between steps | nothing | If at least `moveDelay` seconds passed on `moveClock`: when the snake has a direction, moves the head by `(snakeXSpeed, snakeYSpeed)`, pushes the old head position to the **front** of `tail`, and removes the **last** tail element unless `grow` is set (then clears `grow`). Always restarts `moveClock` once the delay has passed |
| `bool Eat(sf::Vector2f fruitPosition)` | `fruitPosition`: top-left of the fruit | `true` if the head is exactly on the fruit | On a match, sets `grow = true` (the tail keeps its last element on the next step) |
| `void Draw(sf::RenderWindow& window)` | `window` | nothing | Draws the head, then every tail position using the single reused `segment` shape |
| `sf::Vector2f getSnakePosition()` | none | head's top-left position | |
| `sf::Vector2f getSnakeSize()` | none | head's size (20, 20) | Not used anywhere in the project |
| `std::list<sf::Vector2f> getTailPosition()` | none | a **copy** of the tail positions, nearest the head first | Returns by value, so each call copies the whole list |

### Members (internal)

| Member | Type | Purpose |
|---|---|---|
| `snake` | `sf::RectangleShape` | The head |
| `segment` | `sf::RectangleShape` | Template shape redrawn at each tail position |
| `tail` | `std::list<sf::Vector2f>` | Body positions, front = right behind the head. Length equals fruits eaten |
| `moveClock` | `sf::Clock` | Times the steps |
| `SPEED` | `const float` = `20.f` | Pixels per step = one cell |
| `snakeXSpeed`, `snakeYSpeed` | `float`, default `0` | Current velocity per step. Both `0` means "not moving yet" |
| `grow` | `bool`, default `false` | Set by `Eat`, consumed by the next step |

---

## Text

The score, level, and FPS readout.

### Constructor

| Function | Description |
|---|---|
| `Text()` | Loads the font from `"../assets/fonts/HomeVideo-BLG6G.ttf"` (relative to the working directory) and creates three `sf::Text` objects, each size 25. Score and level are white at `(5, 425)` and `(5, 455)`; FPS is green at `(470, 10)`. In SFML 3 a font that cannot be opened throws `sf::Exception`; nothing in the project catches it, so the program ends |

### Methods

| Function | Parameters | Returns | Description |
|---|---|---|---|
| `void Update(int score, int level, float fps)` | current values | nothing | Sets the strings `"SCORE: <n>"`, `"LEVEL: <n>"`, `"FPS: <x.xxx>"` (FPS with 3 decimals) |
| `void Draw(sf::RenderWindow& window)` | `window` | nothing | Draws all three texts |

### Members (internal)

| Member | Type | Purpose |
|---|---|---|
| `font` | `sf::Font` | Must outlive the `sf::Text` objects that use it (it does: same class, declared first) |
| `scoreText`, `levelText`, `fpsText` | `sf::Text` | The three labels |

---

## Game

Owns the window and all game objects, runs the main loop, and decides scoring, collisions, and leveling.

### Public members

| Member | Type | Description |
|---|---|---|
| `window` | `sf::RenderWindow` | 640×480 window titled "Retro Snake" |
| `grid` | `Grid` | |
| `snake` | `Snake` | |
| `fruit` | `Fruit` | |
| `text` | `Text` | |

These are constructed in this order (declaration order): `window`, `grid`, `snake`, `fruit`, `text`, then the private members.

### Public methods

| Function | Parameters | Returns | Description |
|---|---|---|---|
| `Game()` | none | | Creates the window and sets the frame-rate limit to 60 |
| `void Run()` | none | nothing | The main loop. Returns when the window is closed (by the user or by game over). See [the frame loop](04-lifecycle-and-states.md#the-frame-loop) |

### Private methods (internal)

| Function | Returns | Description |
|---|---|---|
| `void HandleEvents()` | nothing | Empties the SFML event queue. A `Closed` event closes the window; a `KeyPressed` event for `H` toggles `showText` |
| `void Render()` | nothing | Despite the name, it **updates** and does not draw: updates the text, reads input, moves the snake, and if the snake ate: `score += 1`, `LevelUp()`, `ValidateFruitCoors()` |
| `void Draw()` | nothing | Draws snake, fruit, grid, then (if `showText`) the text, in that order, so grid lines appear on top of the snake and fruit |
| `void DrawText()` | nothing | Calls `text.Draw(window)` |
| `void ValidateFruitCoors()` | nothing | Re-rolls the fruit position until it is not on the head and not on any tail segment |
| `bool CheckBorderBounds()` | `true` if the head is outside the window | True when head `x < 0` or `x >= 640` or `y < 0` or `y >= 480` |
| `bool CheckHitSnakeBody(sf::Vector2f shapePos)` | `true` if `shapePos` equals any tail position | Used for self-collision (with the head position) and for fruit placement (with the fruit position) |
| `void LevelUp()` | nothing | If `score >= level * 20`: `level += 1`, and if `moveDelay > 0.05f`, `moveDelay -= 0.02f` |

### Private members (internal)

| Member | Type | Initial value | Purpose |
|---|---|---|---|
| `frameClock` | `sf::Clock` | starts at construction | Measures time per frame |
| `deltaTime` | `float` | *uninitialized* until the first loop pass | Seconds of the last frame |
| `fps` | `float` | *uninitialized* until the first loop pass | `1 / deltaTime` |
| `moveDelay` | `float` | `0.15f` | Seconds between snake steps |
| `score` | `int` | `0` | Fruits eaten |
| `level` | `int` | `1` | Current level |
| `showText` | `bool` | `true` | Whether the HUD is drawn |

`deltaTime` and `fps` are assigned in `Run()` before anything reads them, so this is harmless today.

---

## main

`src/Main.cpp`: `int main()` creates one `Game` on the stack, calls `Run()`, and returns `0`. Command-line arguments are not used.
