#include "src/bifurcations.hpp"
#include "src/lorenz.hpp" // Make sure you created this file from our earlier step!
#include <iostream>

int main() {
    std::cout << "Starting Chaos Theory Simulations...\n\n";

    std::cout << "1. Rendering Logistic Map Bifurcation Diagram.\n";
    std::cout << "   (Close the plot window to proceed to the next simulation)\n";
    runBifurcations();

    std::cout << "2. Rendering Lorenz Attractor (Butterfly Effect).\n";
    std::cout << "   (Close the plot window to exit the program)\n";
    runLorenz();

    std::cout << "\nAll simulations complete. Exiting.\n";
    return 0;
}