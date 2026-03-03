#include "bifurcations.hpp"
#include "../utils/plotter.hpp"
#include <vector>

void runBifurcations() {
    std::vector<Point2D> points;
    double rMin = 2.5;
    double rMax = 4.0;
    
    // Increased Density: finer step and more points per R value
    double rStep = 0.0005; // Was 0.001
    int settleIters = 500;
    int plotIters = 800;   // Was 400

    for (double r = rMin; r <= rMax; r += rStep) {
        double x = 0.5;
        for (int i = 0; i < settleIters; ++i) x = r * x * (1.0 - x); 
        for (int i = 0; i < plotIters; ++i) { 
            x = r * x * (1.0 - x);
            points.push_back({r, x});
        }
    }

    Plot2D plotter("Logistic Map Bifurcation Diagram");
    plotter.setLimits(rMin, rMax, 0.0, 1.0);
    plotter.setLabels("Growth Rate (r)", "Population (x)"); // Setting labels
    plotter.setData(points);
    
    plotter.show();
}