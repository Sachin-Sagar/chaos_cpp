#include "bifurcations.hpp"
#include "plotter.hpp"
#include <vector>

Points2DSoA computeBifurcationsSoA() {
    Points2DSoA points;
    double rMin = 2.5;
    double rMax = 4.0;
    double rStep = 0.0005;
    int settleIters = 500;
    int plotIters = 800;

    points.reserve((size_t)((rMax - rMin) / rStep) * plotIters);

    for (double r = rMin; r <= rMax; r += rStep) {
        double x = 0.5;
        for (int i = 0; i < settleIters; ++i) x = r * x * (1.0 - x); 
        for (int i = 0; i < plotIters; ++i) { 
            x = r * x * (1.0 - x);
            points.x.push_back(r);
            points.y.push_back(x);
        }
    }
    return points;
}

void runBifurcationsSoA() {
    auto points = computeBifurcationsSoA();

    Plot2DSoA plotter("Logistic Map Bifurcation Diagram");
    plotter.setLimits(2.5, 4.0, 0.0, 1.0);
    plotter.setLabels("Growth Rate (r)", "Population (x)");
    plotter.setData(points);
    
    plotter.show();
}
