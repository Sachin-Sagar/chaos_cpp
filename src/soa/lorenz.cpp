#include "lorenz.hpp"
#include "../aos/lorenz.hpp" // Reuse Point3D and derivatives for now
#include "plotter.hpp"
#include <vector>

// Note: Reusing scalar derivatives for Phase 2 SoA layout transition
extern Point3D lorenzDerivatives(Point3D p, double s, double r, double b);

Points2DSoA computeLorenzSoA(double dt, int totalSteps) {
    Points2DSoA points;
    points.reserve(totalSteps);

    Point3D p = {0.1, 0.0, 0.0}; 
    double sigma = 10.0;
    double rho = 28.0;
    double beta = 8.0 / 3.0;

    for (int i = 0; i < totalSteps; ++i) {
        Point3D k1 = lorenzDerivatives(p, sigma, rho, beta);
        Point3D p2 = {p.x + k1.x * dt * 0.5, p.y + k1.y * dt * 0.5, p.z + k1.z * dt * 0.5};
        Point3D k2 = lorenzDerivatives(p2, sigma, rho, beta);
        Point3D p3 = {p.x + k2.x * dt * 0.5, p.y + k2.y * dt * 0.5, p.z + k2.z * dt * 0.5};
        Point3D k3 = lorenzDerivatives(p3, sigma, rho, beta);
        Point3D p4 = {p.x + k3.x * dt, p.y + k3.y * dt, p.z + k3.z * dt};
        Point3D k4 = lorenzDerivatives(p4, sigma, rho, beta);

        p.x += (dt / 6.0) * (k1.x + 2*k2.x + 2*k3.x + k4.x);
        p.y += (dt / 6.0) * (k1.y + 2*k2.y + 2*k3.y + k4.y);
        p.z += (dt / 6.0) * (k1.z + 2*k2.z + 2*k3.z + k4.z);

        points.x.push_back(p.x);
        points.y.push_back(p.z);
    }
    return points;
}

void runLorenzSoA() {
    double dt = 0.002;
    int totalSteps = 250000;

    auto points = computeLorenzSoA(dt, totalSteps);

    Plot2DSoA plotter("Lorenz Attractor (X-Z Projection)");
    plotter.setLimits(-30.0, 30.0, 0.0, 55.0);
    plotter.setLabels("X Axis", "Z Axis");
    plotter.setData(points);
    
    plotter.show();
}
