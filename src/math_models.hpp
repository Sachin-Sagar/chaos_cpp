#pragma once
#include <vector>

// A structure to hold 2D mathematical coordiantes
struct Point2D
{
    double x;
    double y;
};

// Generate raw (r,x) data for the logistic map
std::vector<Point2D> computeLogisticMap(double rMin, double rMax, double rStep, int settleIters, int plotIters);