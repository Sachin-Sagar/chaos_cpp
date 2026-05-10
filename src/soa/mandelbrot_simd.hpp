#pragma once
#include <vector>

std::vector<int> computeMandelbrotSIMD(int width, int height, double xMin, double xMax, double yMin, double yMax, int maxIters);
void runMandelbrotSIMD();
