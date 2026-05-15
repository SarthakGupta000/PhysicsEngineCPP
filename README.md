# Physics Engine C++

A minimal 2D rigid-body physics engine with console output. Simulates circles falling under gravity and bouncing off static walls with friction and restitution.

## Requirements

- C++11 or later compiler (e.g., `g++` or `clang++`)
- No external dependencies (uses only the C++ Standard Library)

## Compiling

From the project root directory:

```bash
g++ -std=c++11 main.cpp -o physics_engine
```

For optimised builds:

```bash
g++ -std=c++11 -O2 main.cpp -o physics_engine
```

## Running

```bash
./physics_engine
```

The simulation runs continuously, printing each circle's `(x, y)` position every frame to the console. Press `Ctrl+C` or `Cmd+C` to stop.

## Customising the Simulation

Edit `main.cpp` to change the simulation parameters:

| Parameter | Where to change | Example |
|-----------|-----------------|---------|
| Frame rate | `#define FPS 60` at line 3 | `#define FPS 30` |
| Gravity | `World` constructor at line 12 | `World({0, -9.8}, "Physics Engine")` |
| Friction | `gameLoop` call at line 13, 1st arg (0 = no friction, 1 = full) | `0.1` |
| Restitution | `gameLoop` call at line 13, 2nd arg (0 = inelastic, 1 = perfectly elastic) | `0.9` |
| Circle properties | `Circle(radius, mass, velocity, acceleration, position)` at line 6 | `Circle(1, 10, {1, -1}, {0, 0}, {10, 10})` |
| Wall properties | `Wall(width, height, position)` at line 8 | `Wall(1000, 5, {10, 1})` |
| Number of objects | Array sizes passed to `gameLoop` at line 13 (5th and 7th args) | `walls, 1, circles, 1` |
| Friction per collision | `handleWallCollision` in `util.hpp` — tangential velocity reduced by `(1 - friction)` and normal velocity reversed with `restitution` |

### Adding more objects

```cpp
Circle circle1(1, 10, {1, -1}, {0, 0}, {10, 10});
Circle circle2(2, 5, {-1, 0}, {0, 0}, {5, 5});
Circle *circles = new Circle[2];
circles[0] = circle1;
circles[1] = circle2;

Wall wall1(1000, 5, {10, 1});
Wall *walls = new Wall[1];
walls[0] = wall1;

World world({0, -9.8}, "Physics Engine");
world.gameLoop(0.1, 0.9, FPS, walls, 1, circles, 2);
```

## Class Overview

| Class | Description |
|-------|-------------|
| `RigidObject` | Base class with mass, velocity (2D), acceleration (2D), position (2D). Mass 0 = static. |
| `Circle` | Rigid body with a `radius`. Affected by gravity. |
| `Wall` | Static axis-aligned rectangular obstacle. Always mass 0. |
| `World` | Contains gravity vector and runs the `gameLoop`. |

### Physics loop (`World::gameLoop`)

Uses a fixed-timestep accumulator pattern (`dt = 1/fps`):
1. Apply gravity to velocity (semi-implicit Euler)
2. Update position from velocity
3. Resolve circle-wall collisions (normal bounce with restitution, tangential friction)
4. Sleep for remainder of frame
