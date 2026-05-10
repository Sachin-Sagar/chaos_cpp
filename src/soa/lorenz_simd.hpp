#pragma once
#include <vector>
#include "math_models.hpp"

// Computes 4 simultaneous Lorenz Attractors using AVX2 SIMD
// Returns the points for one of the streams (the primary trajectory)
Points2DSoA computeLorenzSIMD(double dt, int totalSteps);

// Entry point to run the SIMD Lorenz simulation
void runLorenzSIMD();
