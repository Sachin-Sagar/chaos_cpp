#pragma once
#include <vector>

struct ComplexSoA {
    std::vector<double> r;
    std::vector<double> i;

    void reserve(size_t n) {
        r.reserve(n);
        i.reserve(n);
    }
};

std::vector<int> computeMandelbrotSoA(int width, int height, double xMin, double xMax, double yMin, double yMax, int maxIters);
void runMandelbrotSoA();
