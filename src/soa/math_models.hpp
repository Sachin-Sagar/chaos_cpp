#pragma once
#include <vector>

// A structure to hold 2D mathematical coordinates in Structure of Arrays (SoA) format
struct Points2DSoA
{
    std::vector<double> x;
    std::vector<double> y;

    void reserve(size_t n) {
        x.reserve(n);
        y.reserve(n);
    }

    size_t size() const {
        return x.size();
    }
};
