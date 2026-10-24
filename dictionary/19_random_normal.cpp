// title: Normal (Gaussian) random numbers
// category: Random
// python:
//  z = rng.normal(0.0, 1.0)
// cpp:
//  std::normal_distribution<double> N(0.0, 1.0);
//  double z = N(gen);
// note: Used for Brownian increments in SDEs: `dW = std::sqrt(dt) * N(gen)`.
// ---
#include <cmath>
#include <iostream>
#include <random>

int main() {
    std::mt19937 gen(1);
    std::normal_distribution<double> N(0.0, 1.0);
    double s = 0, s2 = 0;
    int n = 100000;
    for (int i = 0; i < n; ++i) { double z = N(gen); s += z; s2 += z * z; }
    double mean = s / n;
    std::cout << "mean = " << mean << ", variance = " << s2 / n - mean * mean << " (theory 0 and 1)\n";
    return 0;
}
