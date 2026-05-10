#pragma once
#include <vector>

struct Complex {
    double r, i;
};

std::vector<int> computeMandelbrotAoS(int width, int height, double xMin, double xMax, double yMin, double yMax, int maxIters);
void runMandelbrot();
