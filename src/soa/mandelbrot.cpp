#include "mandelbrot.hpp"
#include <vector>

std::vector<int> computeMandelbrotSoA(int width, int height, double xMin, double xMax, double yMin, double yMax, int maxIters) {
    std::vector<int> iters;
    iters.reserve(width * height);

    // In a true SoA approach for Mandelbrot, we might pre-calculate all C values
    // But for Mandelbrot, the "Structure" is the complex number (r, i).
    // SoA here means we process Rs and Is from separate arrays if we were doing batch operations.
    // For scalar SoA, it's very similar to AoS but prepares the data layout for SIMD.

    double dx = (xMax - xMin) / width;
    double dy = (yMax - yMin) / height;

    // Pre-calculate C components in SoA format to simulate the layout
    std::vector<double> crs(width);
    for (int x = 0; x < width; ++x) crs[x] = xMin + x * dx;

    for (int y = 0; y < height; ++y) {
        double ci = yMin + y * dy;
        for (int x = 0; x < width; ++x) {
            double cr = crs[x];
            double zr = 0.0;
            double zi = 0.0;
            int count = 0;

            while (count < maxIters && (zr * zr + zi * zi) <= 4.0) {
                double nextZr = zr * zr - zi * zi + cr;
                double nextZi = 2.0 * zr * zi + ci;
                zr = nextZr;
                zi = nextZi;
                count++;
            }
            iters.push_back(count);
        }
    }
    return iters;
}

#include "../../utils/image_plotter.hpp"

void runMandelbrotSoA() {
    ImagePlotter plotter("Mandelbrot Set (SoA Scalar)", computeMandelbrotSoA);
    plotter.show();
}
