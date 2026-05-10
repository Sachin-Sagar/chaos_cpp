# Project: Chaos Theory Simulations in C++

## Project Overview
This project is an interactive, object-oriented C++ application designed to simulate and visualize chaotic dynamical systems. It leverages the **SFML (Simple and Fast Multimedia Library)** for hardware-accelerated 2D rendering. Key simulations include:
- **Logistic Map Bifurcation Diagram:** Visualizes the transition from stable states to chaos through period-doubling.
- **Lorenz Attractor:** Simulates the classic "butterfly effect" system of differential equations using 4th-Order Runge-Kutta (RK4) integration.

The project emphasizes efficient handling of large mathematical datasets using SFML Vertex Arrays, allowing for smooth interactive panning and zooming.

## Architecture & Structure
- `main.cpp`: Entry point orchestrating the sequence of simulations.
- `src/`: Core mathematical models.
    - `math_models.hpp/cpp`: Definitions for 2D/3D points and the Logistic Map logic.
    - `lorenz.hpp/cpp`: Lorenz system parameters and RK4 integration logic.
    - `bifurcations.hpp`: (Implied) Header for logistic map simulation entry.
- `utils/`: Rendering infrastructure and global configuration.
    - `plotter.hpp/cpp`: A generalized `Plot2D` engine for graphing datasets.
    - `settings.hpp`: Global constants for window dimensions, colors, and cross-platform font paths.
- `chaos_theory.html`: Documentation of the underlying mathematical theory.

## Building and Running

### Prerequisites
- C++17 or higher.
- **SFML 2.5+** installed on the system.
- GCC/g++ compiler.

### Build Command (Linux/macOS)
```bash
g++ -O3 main.cpp utils/*.cpp src/*.cpp -o chaos_sim -lsfml-graphics -lsfml-window -lsfml-system
```
*Note: The `-O3` flag is essential for performance due to the high volume of mathematical calculations.*

### Execution
```bash
./chaos_sim
```

## Development Conventions
- **Mathematical Decoupling:** Simulation logic (in `src/`) generates pure mathematical coordinates (`Point2D`), keeping it agnostic of screen pixels or rendering logic.
- **Efficient Rendering:** The `Plot2D` class uses `sf::VertexArray` to batch thousands of points into single draw calls.
- **RK4 Integration:** Prefer 4th-Order Runge-Kutta for differential equations to maintain numerical stability and accuracy.
- **Coordinate Systems:** The plotter handles translation between mathematical space and screen space, including support for zooming and panning.
- **Code Style:** Uses `#pragma once` for header guards and standard STL containers (e.g., `std::vector::reserve`) for performance.

## Interactive Controls
- **Mouse Wheel:** Zoom in/out (centered on view).
- **W/A/S/D:** Pan the camera.
- **R Key:** Reset zoom and pan to default values.
- **P Key:** Save a timestamped snapshot of the current view into the `results/` folder as a `.png` file.
- **Close Window / ESC:** Proceeds to the next simulation or exits the program.
