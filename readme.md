# Chaos Theory Simulations in C++

An interactive, object-oriented C++ visualization tool for exploring chaotic dynamical systems and non-linear dynamics. Inspired by James Gleick's *Chaos*, this project features a custom-built, hardware-accelerated 2D plotting library using SFML.

## Features

* **Custom SFML Plotting Engine:** A generalized `Plot2D` class that handles rendering massive mathematical datasets efficiently using Vertex Arrays.
* **Interactive Controls:**
  * **Zoom:** Mouse wheel zooming with **constant point density** (points remain exactly 1 pixel in size regardless of magnification).
  * **Pan:** `W` `A` `S` `D` keys to smoothly pan around the coordinate system.
  * **Snapshots:** Press `P` to save the current view as a `.png` file.
* **Dynamic UI:** Real-time axis lines, grid, dynamic numeric tick marks, and Unicode-based mathematical equation rendering.
* **Simulations Included:**
  1. **Logistic Map Bifurcation Diagram:** Explores the period-doubling route to chaos ($x_{n+1} = r x_n (1 - x_n)$).
  2. **Lorenz Attractor:** Simulates the "Butterfly Effect" by solving a 3D system of differential equations using the 4th-Order Runge-Kutta (RK4) method and projecting it onto a 2D plane.

## Dependencies

* **Graphics Library:** [SFML 2.5+](https://www.sfml-dev.org/) (Compatible with SFML 3.0)
* **Compiler:** GCC/g++ (C++17 or higher)
* **OS:** Linux, macOS, or Windows (via MSYS2/MinGW)

## Project Structure

```text
Chaos_cpp/
├── build_and_run.sh          # Automated build script (Linux/macOS)
├── main.cpp                  # Entry point (runs simulations sequentially)
├── chaos_theory.html         # HTML documentation of the governing math/theory
├── src/                      # Mathematical models and simulation logic
│   ├── math_models.hpp/cpp   # Shared structures (e.g., Point2D, Point3D)
│   ├── bifurcations.hpp/cpp  # Logistic map data generation
│   └── lorenz.hpp/cpp        # Lorenz system RK4 integration
└── utils/                    # Rendering and infrastructure
    ├── plotter.hpp/cpp       # The Plot2D graphing engine
    └── settings.hpp          # Global configuration (resolution, colors, margins)
```

## Building and Running

### 1. Automated (Linux/macOS)
The easiest way to build and run the simulation is using the provided shell script:
```bash
chmod +x build_and_run.sh
./build_and_run.sh
```

### 2. Manual Compilation
To compile the project manually using GCC:
```bash
g++ -O3 main.cpp utils/*.cpp src/*.cpp -o chaos_sim -lsfml-graphics -lsfml-window -lsfml-system
```

### 3. Execution
```bash
./chaos_sim
```

## Performance & Optimization

This project uses the **`-O3`** optimization flag during compilation. This is critical because:
* **High Iteration Counts:** The Lorenz simulation solves 250,000 steps of differential equations.
* **SIMD Vectorization:** `-O3` allows the compiler to use modern CPU instructions to perform multiple mathematical operations in parallel.
* **Floating Point Efficiency:** Optimizes the heavy 4th-Order Runge-Kutta (RK4) calculations for real-time responsiveness.

## Controls

| Action | Input | Description |
| --- | --- | --- |
| **Zoom In** | `Mouse Wheel Up` | Zooms into the mathematical space centered on the view. |
| **Zoom Out** | `Mouse Wheel Down` | Zooms out of the mathematical space. |
| **Pan Up** | `W` | Moves the camera up along the Y-axis. |
| **Pan Down** | `S` | Moves the camera down along the Y-axis. |
| **Pan Left** | `A` | Moves the camera left along the X-axis. |
| **Pan Right** | `D` | Moves the camera right along the X-axis. |
| **Reset Plot** | `R` | Resets zoom and pan to default values. |
| **Save Snapshot** | `P` | Saves the current graph into the `results/` folder as a timestamped `.png` file. |
| **Close Plot** | `Window 'X' Button` or `ESC` | Closes the current simulation and continues to the next. |

## Documentation

For a detailed mathematical explanation of the systems modeled in this project, open `chaos_theory.html` in any modern web browser. It features MathJax-rendered equations and stability analysis for the simulated attractors.
