#include "mandelbrot.hpp"
#include <vector>

#include <iostream>

std::vector<int> computeMandelbrotAoS(int width, int height, double xMin, double xMax, double yMin, double yMax, int maxIters) {
    std::vector<int> iters;
    iters.reserve(width * height);

    double dx = (xMax - xMin) / width;
    double dy = (yMax - yMin) / height;

    int nonZeroCount = 0;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double cr = xMin + x * dx;
            double ci = yMin + y * dy;

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

// runMandelbrot will be implemented once ImagePlotter is ready
void runMandelbrot() {
    ImagePlotter plotter("Mandelbrot Set (AoS Scalar)", computeMandelbrotAoS);
    plotter.show();
}
