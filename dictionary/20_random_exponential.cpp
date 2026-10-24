// title: Exponential waiting times
// category: Random
// python:
//  tau = rng.exponential(1 / rate)
// cpp:
//  std::exponential_distribution<double> wait(rate);
//  double tau = wait(gen);
// note: Careful: NumPy takes the mean (1/rate), C++ takes the rate itself. This is the waiting time to the next event in the Gillespie algorithm.
// ---
#include <iostream>
#include <random>

int main() {
    std::mt19937 gen(3);
    double rate = 2.0;
    std::exponential_distribution<double> wait(rate);
    double s = 0;
    int n = 100000;
    for (int i = 0; i < n; ++i) s += wait(gen);
    std::cout << "mean waiting time = " << s / n << " (theory 1/rate = " << 1 / rate << ")\n";
    return 0;
}
