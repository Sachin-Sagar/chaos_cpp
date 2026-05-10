# Chaos Theory Simulations in C++ (SIMD Optimization Lab)

An interactive, object-oriented C++ visualization tool for exploring chaotic dynamical systems, specifically redesigned to facilitate learning **SIMD (Single Instruction, Multiple Data)** optimizations. 

This project serves as a practical laboratory for comparing standard scalar C++ with manual **AVX2 Intrinsics**, demonstrating significant real-world performance gains in mathematical modeling and graphics rendering.

## Features

*   **Dual-Architecture Implementation:** 
    *   **AoS (Array of Structures):** The standard C++ approach using objects like `Point2D` or `Complex`.
    *   **SoA (Structure of Arrays):** A SIMD-friendly layout where components are stored in contiguous arrays, enabling 256-bit register throughput.
*   **Custom SFML Plotting Engine:** 
    *   **Scatter Plotter:** For coordinate-based data (Bifurcations, Lorenz).
    *   **Fractal Image Plotter:** A dedicated engine for grid-based data (Mandelbrot) using `sf::Image` and `sf::Texture`.
*   **Benchmarking Suite:** A command-line mode to measure raw mathematical throughput and rendering efficiency without GUI overhead.
*   **Simulations Included:**
    1.  **Logistic Map Bifurcation Diagram:** Explores the period-doubling route to chaos ($x_{n+1} = r x_n (1 - x_n)$).
    2.  **Lorenz Attractor:** Simulates the "Butterfly Effect" using 4th-Order Runge-Kutta (RK4) integration.
    3.  **Mandelbrot Set:** A compute-intensive escape-time fractal that serves as the primary showcase for SIMD throughput.

## Project Structure

```text
Chaos_cpp/
├── main.cpp                  # Entry point (handles CLI arguments and benchmarking)
├── build_and_run.sh          # Automated build script (Linux/macOS)
├── utils/                    # Shared utilities
│   ├── settings.hpp          # Global configuration (resolution, colors, etc.)
│   ├── timer.hpp             # High-resolution RAII timer for benchmarking
│   └── image_plotter.hpp/cpp # Fractal rendering engine (SFML 3.0)
├── src/
│   ├── aos/                  # Phase 1: Baseline Array of Structures (Scalar)
│   │   ├── math_models.hpp/cpp
│   │   ├── bifurcations.hpp/cpp
│   │   ├── lorenz.hpp/cpp
│   │   ├── mandelbrot.hpp/cpp
│   │   └── plotter.hpp/cpp   # AoS-specific renderer
│   └── soa/                  # Phase 2-5: Structure of Arrays & SIMD
│       ├── math_models.hpp   # Defines Points2DSoA
│       ├── bifurcations.hpp/cpp (Scalar & AVX2 SIMD)
│       ├── lorenz.hpp/cpp       (Scalar & AVX2 Stream SIMD)
│       ├── mandelbrot_simd.hpp/cpp (AVX2 Vectorized Escape-Time)
│       └── plotter.hpp/cpp      (SIMD-Accelerated Renderer)
└── results/                  # Snapshot storage folder
```

## Building and Running

### 1. Automated (Linux/macOS)
The script automatically enables `-O3` and `-mavx2` flags:
```bash
chmod +x build_and_run.sh
./build_and_run.sh
```

### 2. Performance Benchmarking
To run the side-by-side comparison of all optimization stages:
```bash
./chaos_sim --benchmark
```

## Optimization Roadmap (Laboratory Results)

| Phase | Milestone | Result | Key Takeaway |
| --- | --- | --- | --- |
| **1** | **Benchmarking** | Baseline Established | If you don't measure, you aren't optimizing. |
| **2** | **SoA Layout** | SIMD Readiness | Reorganizing data in memory is required for register efficiency. |
| **3** | **Manual SIMD (Math)** | ~27% Gain (at -O1) | Explicit intrinsics beat unoptimized scalar code. |
| **4** | **Stream SIMD (Lorenz)** | **3x Speedup** | Parallelizing independent data streams (universes) unlocks SIMD's full power. |
| **5** | **Fractal Vectorization** | **3.4x Speedup** | The Mandelbrot set proves that high arithmetic intensity is the "sweet spot" for SIMD. |
| **6** | **SIMD Rendering** | **2x Speedup** | SIMD accelerates the graphics pipeline (Double-to-Float & coordinate math). |

## Controls

| Action | Input | Description |
| --- | --- | --- |
| **Zoom** | `Mouse Wheel` | Zooms into the mathematical space. |
| **Pan** | `W/A/S/D` | Smoothly pan around the coordinate system. |
| **Reset** | `R` | Resets zoom and pan to default values. |
| **Snapshot** | `P` | Saves the current graph into `results/` as a `.png`. |
| **Exit** | `ESC` | Closes the current simulation. |

## Performance & Optimization

This project demonstrates two levels of optimization:
1.  **Auto-Vectorization:** Leveraged by the compiler (`-O3`) in the AoS implementation.
2.  **Explicit Vectorization:** Using **AVX2 Intrinsics** (`<immintrin.h>`) in the SoA implementation.

**Key Laboratory Finding:** While the compiler is excellent at simple loops (Logistic Map), manual stream vectorization (Phase 4) and custom coordinate transformations (Phase 5) consistently outperform even the highest compiler optimization levels by exploiting the hardware's 256-bit wide registers.
