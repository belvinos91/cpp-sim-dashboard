// Geometric Brownian motion dX = mu X dt + sigma X dW, Euler-Maruyama.
// Plots 10 paths; prints the mean of X(T) over many paths vs the exact e^(mu T).
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

int main() {
    double mu = 0.5, sigma = 0.4, x0 = 1, T = 2;
    int steps = 500, paths = 5000, shown = 10;
    double dt = T / steps;
    std::mt19937 gen(42);                          // fixed seed: the run is reproducible
    std::normal_distribution<double> N01(0.0, 1.0);

    std::vector<std::vector<double>> keep(shown, std::vector<double>(steps + 1, x0));
    double sum = 0, sumSq = 0;
    for (int p = 0; p < paths; ++p) {
        double x = x0;
        for (int n = 1; n <= steps; ++n) {
            x += mu * x * dt + sigma * x * std::sqrt(dt) * N01(gen);
            if (p < shown) keep[p][n] = x;
        }
        sum += x; sumSq += x * x;
    }
    std::ofstream out("data.csv");
    out << "t";
    for (int p = 1; p <= shown; ++p) out << ",path " << p;
    out << "\n";
    for (int n = 0; n <= steps; ++n) {
        out << n * dt;
        for (int p = 0; p < shown; ++p) out << "," << keep[p][n];
        out << "\n";
    }
    double mean = sum / paths, se = std::sqrt((sumSq / paths - mean * mean) / paths);
    std::cout << "mean X(T) = " << mean << " +/- " << 2 * se
              << "   exact = " << x0 * std::exp(mu * T) << "\n";
    return 0;
}
