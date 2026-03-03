Here is a complete `README.md` file for your project. It summarizes the purpose, features, dependencies, and build instructions, making it easy for anyone (or yourself in the future) to understand how to run and explore your chaos simulations.

```markdown
# Chaos Theory Simulations in C++

An interactive, object-oriented C++ visualization tool for exploring chaotic dynamical systems and non-linear dynamics. Inspired by James Gleick's *Chaos*, this project features a custom-built, hardware-accelerated 2D plotting library using SFML.

## Features

* **Custom SFML Plotting Engine:** A generalized `Plot2D` class that handles rendering massive mathematical datasets efficiently using Vertex Arrays.
* **Interactive Controls:** * **Zoom:** Mouse wheel zooming with **constant point density** (points remain exactly 1 pixel in size regardless of magnification).
  * **Pan:** `W` `A` `S` `D` keys to smoothly pan around the coordinate system.
* **Dynamic UI:** Real-time axis lines, grid, dynamic numeric tick marks, and Unicode-based mathematical equation rendering.
* **Simulations Included:**
  1. **Logistic Map Bifurcation Diagram:** Explores the period-doubling route to chaos ($x_{n+1} = r x_n (1 - x_n)$).
  2. **Lorenz Attractor:** Simulates the "Butterfly Effect" by solving a 3D system of differential equations using the 4th-Order Runge-Kutta (RK4) method and projecting it onto a 2D plane.

## Dependencies

* **OS:** Windows (easily adaptable to Linux/macOS)
* **Compiler:** GCC/g++ (configured via MSYS2 UCRT64)
* **Graphics Library:** [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)

## Project Structure

```text
Chaos_cpp/
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

### 1. Visual Studio Code (Tasks)

If you are using the configured VS Code workspace, simply press `Ctrl + Shift + B` (or run your default build task) to compile all `.cpp` files in the workspace.

### 2. Command Line (MSYS2)

To compile the project manually using GCC, run the following command from the root project directory:

```bash
g++ -O3 main.cpp utils/*.cpp src/*.cpp -o main.exe -lsfml-graphics -lsfml-window -lsfml-system

```

*(Note: The `-O3` flag is highly recommended to optimize the heavy mathematical loops for the RK4 solver and point generation).*

### 3. Execution

Run the compiled executable:

```bash
./main.exe

```

The console will guide you through the simulations. Close the current plot window to progress to the next simulation.

## Controls

| Action | Input | Description |
| --- | --- | --- |
| **Zoom In** | `Mouse Wheel Up` | Zooms into the mathematical space centered on the view. |
| **Zoom Out** | `Mouse Wheel Down` | Zooms out of the mathematical space. |
| **Pan Up** | `W` | Moves the camera up along the Y-axis. |
| **Pan Down** | `S` | Moves the camera down along the Y-axis. |
| **Pan Left** | `A` | Moves the camera left along the X-axis. |
| **Pan Right** | `D` | Moves the camera right along the X-axis. |
| **Close Plot** | `Window 'X' Button` | Closes the current simulation and continues to the next. |

## Documentation

For a detailed mathematical explanation of the systems modeled in this project, open `chaos_theory.html` in any modern web browser. It features MathJax-rendered equations and stability analysis for the simulated attractors.

