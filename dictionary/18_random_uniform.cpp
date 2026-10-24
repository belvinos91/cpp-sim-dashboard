// title: Random numbers with a seed
// category: Random
// python:
//  rng = np.random.default_rng(42)
//  u = rng.uniform(0, 1)
// cpp:
//  std::mt19937 gen(42);
//  std::uniform_real_distribution<double> U(0.0, 1.0);
//  double u = U(gen);
// note: In C++ the generator (`std::mt19937`, seeded) and the distribution are separate objects. Create the generator once and reuse it; the same seed gives the same numbers every run.
// ---
#include <iostream>
#include <random>

int main() {
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> U(0.0, 1.0);
    double sum = 0;
    int n = 100000;
    for (int i = 0; i < n; ++i) sum += U(gen);
    std::cout << "mean of " << n << " uniforms = " << sum / n << " (theory 0.5)\n";
    return 0;
}
