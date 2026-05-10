#include "lorenz_simd.hpp"
#include "plotter.hpp"
#include <immintrin.h>
#include <vector>

// Helper structure for 4 simultaneous 3D points
struct Point3D_x4 {
    __m256d x, y, z;
};

// Vectorized derivatives for 4 streams
inline Point3D_x4 lorenzDerivativesSIMD(Point3D_x4 p, __m256d s, __m256d r, __m256d b) {
    // dx = s * (y - x)
    // dy = x * (r - z) - y
    // dz = x * y - b * z
    return {
        _mm256_mul_pd(s, _mm256_sub_pd(p.y, p.x)),
        _mm256_sub_pd(_mm256_mul_pd(p.x, _mm256_sub_pd(r, p.z)), p.y),
        _mm256_sub_pd(_mm256_mul_pd(p.x, p.y), _mm256_mul_pd(b, p.z))
    };
}

Points2DSoA computeLorenzSIMD(double dt, int totalSteps) {
    Points2DSoA points;
    points.reserve(totalSteps);

    // Initial conditions for 4 streams (slightly offset to show butterfly effect)
    Point3D_x4 p = {
        _mm256_set_pd(0.1, 0.1001, 0.1002, 0.1003),
        _mm256_set1_pd(0.0),
        _mm256_set1_pd(0.0)
    };

    __m256d sigma = _mm256_set1_pd(10.0);
    __m256d rho = _mm256_set1_pd(28.0);
    __m256d beta = _mm256_set1_pd(8.0 / 3.0);
    __m256d vdt = _mm256_set1_pd(dt);
    __m256d vdt05 = _mm256_set1_pd(dt * 0.5);
    __m256d vdt_6 = _mm256_set1_pd(dt / 6.0);
    __m256d two = _mm256_set1_pd(2.0);

    alignas(32) double x_out[4];
    alignas(32) double z_out[4];

    for (int i = 0; i < totalSteps; ++i) {
        // RK4 Integration for 4 universes at once
        Point3D_x4 k1 = lorenzDerivativesSIMD(p, sigma, rho, beta);
        
        Point3D_x4 p_k2 = {
            _mm256_add_pd(p.x, _mm256_mul_pd(k1.x, vdt05)),
            _mm256_add_pd(p.y, _mm256_mul_pd(k1.y, vdt05)),
            _mm256_add_pd(p.z, _mm256_mul_pd(k1.z, vdt05))
        };
        Point3D_x4 k2 = lorenzDerivativesSIMD(p_k2, sigma, rho, beta);

        Point3D_x4 p_k3 = {
            _mm256_add_pd(p.x, _mm256_mul_pd(k2.x, vdt05)),
            _mm256_add_pd(p.y, _mm256_mul_pd(k2.y, vdt05)),
            _mm256_add_pd(p.z, _mm256_mul_pd(k2.z, vdt05))
        };
        Point3D_x4 k3 = lorenzDerivativesSIMD(p_k3, sigma, rho, beta);

        Point3D_x4 p_k4 = {
            _mm256_add_pd(p.x, _mm256_mul_pd(k3.x, vdt)),
            _mm256_add_pd(p.y, _mm256_mul_pd(k3.y, vdt)),
            _mm256_add_pd(p.z, _mm256_mul_pd(k3.z, vdt))
        };
        Point3D_x4 k4 = lorenzDerivativesSIMD(p_k4, sigma, rho, beta);

        // Final weighted sum
        __m256d dx = _mm256_mul_pd(vdt_6, _mm256_add_pd(k1.x, _mm256_add_pd(_mm256_mul_pd(two, k2.x), _mm256_add_pd(_mm256_mul_pd(two, k3.x), k4.x))));
        __m256d dy = _mm256_mul_pd(vdt_6, _mm256_add_pd(k1.y, _mm256_add_pd(_mm256_mul_pd(two, k2.y), _mm256_add_pd(_mm256_mul_pd(two, k3.y), k4.y))));
        __m256d dz = _mm256_mul_pd(vdt_6, _mm256_add_pd(k1.z, _mm256_add_pd(_mm256_mul_pd(two, k2.z), _mm256_add_pd(_mm256_mul_pd(two, k3.z), k4.z))));

        p.x = _mm256_add_pd(p.x, dx);
        p.y = _mm256_add_pd(p.y, dy);
        p.z = _mm256_add_pd(p.z, dz);

        // Store and extract only the first stream for visualization
        _mm256_store_pd(x_out, p.x);
        _mm256_store_pd(z_out, p.z);
        points.x.push_back(x_out[0]);
        points.y.push_back(z_out[0]);
    }
    return points;
}

void runLorenzSIMD() {
    auto points = computeLorenzSIMD(0.002, 250000);
    Plot2DSoA plotter("Lorenz Attractor (SIMD Stream)");
    plotter.setLimits(-30.0, 30.0, 0.0, 55.0);
    plotter.setLabels("X Axis", "Z Axis");
    plotter.setData(points);
    plotter.show();
}
