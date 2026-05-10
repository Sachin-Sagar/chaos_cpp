#pragma once
#include "math_models.hpp"

// Computes the Logistic Map data using manual AVX2 SIMD instructions
Points2DSoA computeBifurcationsSIMD();

// Entry point to run the SIMD Bifurcations simulation
void runBifurcationsSIMD();
