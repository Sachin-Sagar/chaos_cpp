#pragma once
#include <vector>
#include "math_models.hpp"

// Computes the Lorenz Attractor points using SoA layout
Points2DSoA computeLorenzSoA(double dt, int totalSteps);

// Entry point to run the Lorenz simulation with SoA rendering
void runLorenzSoA();
