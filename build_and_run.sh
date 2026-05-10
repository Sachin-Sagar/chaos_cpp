#!/bin/bash

# Exit on any error
set -e

echo "--- Compiling Chaos Theory Simulations ---"

# Compilation command
# -O3: Aggressive optimization for mathematical loops
# main.cpp: Entry point
# utils/*.cpp: Plotting engine and settings
# src/*.cpp: Simulation logic (Lorenz, Bifurcations)
# -o chaos_sim: Output binary name
# -lsfml-*: SFML library links
g++ -O3 -mavx2 main.cpp utils/*.cpp src/aos/*.cpp src/soa/*.cpp -o chaos_sim -lsfml-graphics -lsfml-window -lsfml-system

echo "--- Compilation Successful ---"
echo "--- Launching Simulation ---"
echo "Controls: W/A/S/D to Pan, Mouse Wheel to Zoom, 'P' to save PNG, 'ESC' to exit."
echo "Use './chaos_sim --benchmark' for performance testing."

./chaos_sim "$@"
