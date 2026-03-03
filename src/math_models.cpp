#include "math_models.hpp"

std::vector<Point2D> computeLogisticMap(double rMin, double rMax, double rStep, int settleIters, int plotIters)
{

    std::vector<Point2D> points;

    // Pre-allocate memory to speed up vector operations
    points.reserve(static_cast<size_t> ((rMax-rMin)/rStep)*plotIters);

    for (double r = rMin; r <= rMax; r += rStep)
    {
        double x = 0.5;             // Initial population 

        // 1. Settle
        for(int i = 0; i < settleIters; ++i)
            x = r*x*(1-x);

        // 2. Collect Points
        for(int i = 0; i < plotIters; ++i)
        {

            x = r*x*(1-x);
            points.push_back({r,x});    // Store pure math data, completely unaware of pixels

        }

    }

    return points;

}