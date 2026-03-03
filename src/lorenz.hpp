#pragma once
#include <vector>
#include "math_models.hpp" // To reuse Point2D for the final plot

struct Point3D {
    double x, y, z;
};

// Computes the Lorenz Attractor points and projects them to 2D
std::vector<Point2D> computeLorenz(double dt, int totalSteps);

// Entry point to run the Lorenz simulation
void runLorenz();