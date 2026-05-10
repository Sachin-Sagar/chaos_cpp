#include "bifurcations_simd.hpp"
#include "plotter.hpp"
#include <immintrin.h>
#include <vector>
#include <iostream>

Points2DSoA computeBifurcationsSIMD() {
    Points2DSoA points;
    double rMin = 2.5;
    double rMax = 4.0;
    double rStep = 0.0005;
    int settleIters = 500;
    int plotIters = 800;

    size_t numR = (size_t)((rMax - rMin) / rStep) + 1;
    size_t totalPoints = numR * plotIters;
    points.x.resize(totalPoints);
    points.y.resize(totalPoints);

    double* xPtr = points.y.data();
    double* rPtr = points.x.data();

    __m256d one = _mm256_set1_pd(1.0);
    __m256d rOffsets = _mm256_set_pd(rStep * 3.0, rStep * 2.0, rStep, 0.0);

    for (size_t rIdx = 0; rIdx < numR; rIdx += 4) {
        double rBase = rMin + rIdx * rStep;
        __m256d r = _mm256_add_pd(_mm256_set1_pd(rBase), rOffsets);
        __m256d x = _mm256_set1_pd(0.5);

        // 1. Settle Loop (SIMD)
        for (int i = 0; i < settleIters; ++i) {
            __m256d tmp = _mm256_sub_pd(one, x);
            x = _mm256_mul_pd(r, _mm256_mul_pd(x, tmp));
        }

        // 2. Collection Loop (SIMD)
        // Store all plotIters for these 4 R-values in a local buffer first
        // to avoid interleaved extraction logic.
        alignas(32) double temp_x[800 * 4]; 
        
        for (int i = 0; i < plotIters; ++i) {
            __m256d tmp = _mm256_sub_pd(one, x);
            x = _mm256_mul_pd(r, _mm256_mul_pd(x, tmp));
            _mm256_store_pd(&temp_x[i * 4], x);
        }

        // 3. Extraction (Non-SIMD but cache friendly)
        alignas(32) double r_vals[4];
        _mm256_store_pd(r_vals, r);

        for (int j = 0; j < 4; ++j) {
            if (rIdx + j < numR) {
                size_t blockBase = (rIdx + j) * plotIters;
                for (int i = 0; i < plotIters; ++i) {
                    rPtr[blockBase + i] = r_vals[j];
                    xPtr[blockBase + i] = temp_x[i * 4 + j];
                }
            }
        }
    }
    return points;
}

void runBifurcationsSIMD() {
    auto points = computeBifurcationsSIMD();

    Plot2DSoA plotter("Logistic Map Bifurcation Diagram (SIMD)");
    plotter.setLimits(2.5, 4.0, 0.0, 1.0);
    plotter.setLabels("Growth Rate (r)", "Population (x)");
    plotter.setData(points);
    
    plotter.show();
}
