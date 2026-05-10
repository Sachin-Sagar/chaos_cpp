#include "src/aos/bifurcations.hpp"
#include "src/aos/lorenz.hpp"
#include "src/soa/bifurcations.hpp"
#include "src/soa/lorenz.hpp"
#include "src/soa/bifurcations_simd.hpp"
#include "src/soa/lorenz_simd.hpp"
#include "src/soa/plotter.hpp"
#include "utils/timer.hpp"
#include <iostream>
#include <string>
#include <vector>

void runBenchmark() {
    std::cout << "--- Starting Performance Benchmark ---\n";
    
    std::cout << "\n[AoS Implementation]\n";
    {
        ScopeTimer timer("AoS: Logistic Map");
        auto points = computeBifurcations();
        std::cout << "    Generated " << points.size() << " points.\n";
    }

    {
        ScopeTimer timer("AoS: Lorenz Attractor");
        auto points = computeLorenz(0.002, 250000);
        std::cout << "    Generated " << points.size() << " points.\n";
    }

    std::cout << "\n[SoA Implementation]\n";
    {
        ScopeTimer timer("SoA: Logistic Map (Scalar)");
        auto points = computeBifurcationsSoA();
        std::cout << "    Generated " << points.size() << " points.\n";
    }

    {
        ScopeTimer timer("SoA: Logistic Map (AVX2 SIMD)");
        auto points = computeBifurcationsSIMD();
        std::cout << "    Generated " << points.size() << " points.\n";
    }

    {
        ScopeTimer timer("SoA: Lorenz Attractor (Scalar)");
        auto points = computeLorenzSoA(0.002, 250000);
        std::cout << "    Generated " << points.size() << " points.\n";
    }

    {
        ScopeTimer timer("SoA: Lorenz Attractor (AVX2 SIMD)");
        auto points = computeLorenzSIMD(0.002, 250000);
        std::cout << "    Generated " << points.size() << " points.\n";
    }

    std::cout << "\n[Rendering Benchmark (SoA SIMD Plotter)]\n";
    {
        auto points = computeBifurcationsSIMD();
        Plot2DSoA plotter("Benchmark Plot");
        plotter.setData(points);
        ScopeTimer timer("SIMD Plotter: 100 Render Passes");
        plotter.benchmarkRender(100);
    }

    std::cout << "\n--- Benchmark Complete ---\n";
}

int main(int argc, char* argv[]) {
    bool benchmarkMode = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--benchmark") {
            benchmarkMode = true;
        }
    }

    if (benchmarkMode) {
        runBenchmark();
    } else {
        std::cout << "Starting Chaos Theory Simulations (AoS Mode)...\n\n";

        std::cout << "1. Rendering Logistic Map Bifurcation Diagram.\n";
        runBifurcations();

        std::cout << "2. Rendering Lorenz Attractor (Butterfly Effect).\n";
        runLorenz();

        std::cout << "\nAll simulations complete. Exiting.\n";
    }

    return 0;
}
