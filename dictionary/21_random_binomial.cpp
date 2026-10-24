// title: Binomial draws (tau-leaping)
// category: Random
// python:
//  k = rng.binomial(n, p)
// cpp:
//  std::binomial_distribution<int> B(n, p);
//  int k = B(gen);
// note: One binomial draw replaces `n` Bernoulli draws, which makes tau-leaping simulations much faster. The distribution object holds `n` and `p`, so create a new one when they change.
// ---
#include <iostream>
#include <random>

int main() {
    std::mt19937 gen(5);
    int n = 120;
    double p = 0.05;
    double s = 0;
    int reps = 100000;
    for (int r = 0; r < reps; ++r) {
        std::binomial_distribution<int> B(n, p);
        s += B(gen);
    }
    std::cout << "mean = " << s / reps << " (theory n*p = " << n * p << ")\n";
    return 0;
}
