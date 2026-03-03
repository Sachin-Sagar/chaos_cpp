#include "lorenz.hpp"
#include "../utils/plotter.hpp"
#include <vector>

// Derivatives for the Lorenz system
Point3D lorenzDerivatives(Point3D p, double s, double r, double b) {
    return {
        s * (p.y - p.x),
        p.x * (r - p.z) - p.y,
        p.x * p.y - b * p.z
    };
}

std::vector<Point2D> computeLorenz(double dt, int totalSteps) {
    std::vector<Point2D> points;
    points.reserve(totalSteps);

    // Initial conditions and parameters
    Point3D p = {0.1, 0.0, 0.0}; 
    double sigma = 10.0;
    double rho = 28.0;
    double beta = 8.0 / 3.0;

    for (int i = 0; i < totalSteps; ++i) {
        // Runge-Kutta 4th Order Integration
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

        // Project 3D (x, z) to 2D for the "Butterfly" view
        points.push_back({p.x, p.z});
    }
    return points;
}

void runLorenz() {
    // Increased Density: 
    // Smaller dt (time step) for smoother curves, 
    // Higher totalSteps to compensate and draw the full shape.
    double dt = 0.002;       // Was 0.01
    int totalSteps = 250000; // Was 50,000

    auto points = computeLorenz(dt, totalSteps);

    Plot2D plotter("Lorenz Attractor (X-Z Projection)");
    
    // Limits: X ranges roughly -25 to 25, Z ranges 0 to 55
    plotter.setLimits(-30.0, 30.0, 0.0, 55.0);
    plotter.setLabels("X Axis", "Z Axis");
    plotter.setData(points);
    
    plotter.show();
}