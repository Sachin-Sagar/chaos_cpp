#include "mandelbrot_simd.hpp"
#include <immintrin.h>
#include <vector>

std::vector<int> computeMandelbrotSIMD(int width, int height, double xMin, double xMax, double yMin, double yMax, int maxIters) {
    std::vector<int> iters(width * height);

    double dx = (xMax - xMin) / width;
    double dy = (yMax - yMin) / height;

    __m256d vFour = _mm256_set1_pd(4.0);
    __m256d vTwo = _mm256_set1_pd(2.0);
    __m256d vDx = _mm256_set1_pd(dx);
    __m256d vXMin = _mm256_set1_pd(xMin);

    for (int y = 0; y < height; ++y) {
        double ci_val = yMin + y * dy;
        __m256d vCi = _mm256_set1_pd(ci_val);

        for (int x = 0; x < width; x += 4) {
            // Setup Cr for 4 pixels
            __m256d vX = _mm256_set_pd(x + 3, x + 2, x + 1, x);
            __m256d vCr = _mm256_add_pd(vXMin, _mm256_mul_pd(vX, vDx));

            __m256d vZr = _mm256_set1_pd(0.0);
            __m256d vZi = _mm256_set1_pd(0.0);
            __m256d vCount = _mm256_set1_pd(0.0);
            
            // Initial mask: all lanes active (all bits 1)
            // Correct way to get all bits 1 in AVX: compare a register with itself
            __m256d vActiveMask = _mm256_cmp_pd(vZr, vZr, _CMP_EQ_OQ);

            for (int i = 0; i < maxIters; ++i) {
                __m256d vZr2 = _mm256_mul_pd(vZr, vZr);
                __m256d vZi2 = _mm256_mul_pd(vZi, vZi);
                __m256d vMag2 = _mm256_add_pd(vZr2, vZi2);

                // Check escape condition: Mag2 <= 4.0
                __m256d vCond = _mm256_cmp_pd(vMag2, vFour, _CMP_LE_OQ);
                
                // Update active mask: pixel is active if it was active AND it hasn't escaped
                vActiveMask = _mm256_and_pd(vActiveMask, vCond);

                // If no pixels are active, break early
                if (_mm256_testz_pd(vActiveMask, vActiveMask)) break;

                // Increment counts for active pixels
                // vActiveMask has all bits 1 for active lanes. 
                // We use blend to add 1.0 to vCount only for active lanes.
                __m256d vOne = _mm256_set1_pd(1.0);
                __m256d vInc = _mm256_and_pd(vActiveMask, vOne);
                vCount = _mm256_add_pd(vCount, vInc);

                // Next Z
                __m256d vNextZr = _mm256_add_pd(_mm256_sub_pd(vZr2, vZi2), vCr);
                __m256d vNextZi = _mm256_add_pd(_mm256_mul_pd(vTwo, _mm256_mul_pd(vZr, vZi)), vCi);
                
                vZr = vNextZr;
                vZi = vNextZi;
            }

            // Store results
            alignas(32) double res[4];
            _mm256_store_pd(res, vCount);
            for (int k = 0; k < 4; ++k) {
                if (x + k < width) {
                    iters[y * width + (x + k)] = (int)res[k];
                }
            }
        }
    }
    return iters;
}

#include "../../utils/image_plotter.hpp"

void runMandelbrotSIMD() {
    ImagePlotter plotter("Mandelbrot Set (AVX2 SIMD)", computeMandelbrotSIMD);
    plotter.show();
}
