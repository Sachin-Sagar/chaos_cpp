#pragma once
#include <vector>
#include "math_models.hpp"

// Computes the Logistic Map data using Structure of Arrays (SoA) layout
Points2DSoA computeBifurcationsSoA();

// Entry point to run the Bifurcations simulation with SoA rendering
void runBifurcationsSoA();
